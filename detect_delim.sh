#!/usr/bin/env bash
# detect_delim - 自动检测分隔符 & 编码，支持列号/列名(模糊匹配)提取、转CSV、全文件结构检测（单文件版）

if [ $# -lt 1 ]; then
    echo "用法:"
    echo "  $0 <文件路径>"
    echo "  $0 <文件路径> <列号,...>"
    echo "  $0 <文件路径> <列名,...>"
    echo "  $0 <文件路径> csv"
    echo "  $0 <文件路径> check"
    echo "  $0 <文件路径> head"
    echo "  $0 <文件路径> stats"
    echo "  $0 <文件路径> dedup"
    echo "  $0 <文件路径> duplicates"
    echo "  $0 <文件路径> split [分隔符]"
    echo "  $0 \"字符串\" split [分隔符]"
    exit 1
fi

file="$1"
arg2="$2"
arg3="$3"
tmpfile=$(mktemp)

# 字符串拆分功能
if [ "$arg2" = "split" ]; then
    # 如果第一个参数不是文件，则作为字符串处理
    if [ ! -f "$file" ]; then
        input_string="$file"
        # 默认分隔符：中文逗号、英文逗号、分号、竖线、空格
        if [ -n "$arg3" ]; then
            split_delim="$arg3"
        else
            split_delim="[、,;| ]+"
        fi
        
        echo "$input_string" | awk -v RS="$split_delim" '{
            if(NF > 0 && $0 != "") {
                gsub(/^[ \t]+|[ \t]+$/, "", $0)  # 去除首尾空白
                if($0 != "") print $0
            }
        }'
        exit 0
    fi
fi

if [ ! -f "$file" ]; then
    echo "文件不存在: $file"
    rm -f "$tmpfile"
    exit 1
fi

# 编码检测并转换为UTF-8
encoding=$(file -bi "$file" | sed -n 's/.*charset=\(.*\)$/\1/p' | tr '[:upper:]' '[:lower:]')
if [ "$encoding" != "utf-8" ]; then
    iconv -f "$encoding" -t utf-8 "$file" -o "$tmpfile" 2>/dev/null || {
        echo "编码转换失败: $encoding"
        rm -f "$tmpfile"
        exit 1
    }
    src="$tmpfile"
else
    src="$file"
fi

# 检测分隔符
delim=$(awk 'NR==1{
    for(i=1;i<=length($0);i++){
        c=substr($0,i,1)
        if(c ~ /[\t,;| ]/) sep[c]++
    }
}
END{
    max=0
    for(s in sep) if(sep[s]>max){max=sep[s]; sym=s}
    if(sym=="\t") sym="TAB"
    print sym
}' "$src")

if [ "$delim" = " " ]; then
    if grep -q '  \+' "$src"; then
        delim="MULTISPACE"
    fi
fi

case "$delim" in
    TAB) awk_delim='\t' ;;
    MULTISPACE) awk_delim='[ ]+' ;;
    *) awk_delim="$delim" ;;
esac

# 仅检测分隔符
if [ -z "$arg2" ]; then
    echo "$delim"
    rm -f "$tmpfile"
    exit 0
fi

# 新增功能：输出列名和对应的列数
if [ "$arg2" = "head" ]; then
    awk -v FS="$awk_delim" '
    NR==1 {
        print "列名和对应的列号:"
        for(i=1; i<=NF; i++) {
            print i ": " $i
        }
        exit
    }' "$src"
    rm -f "$tmpfile"
    exit 0
fi

# 文件内容拆分功能
if [ "$arg2" = "split" ]; then
    # 默认分隔符：中文逗号、英文逗号、分号、竖线、空格
    if [ -n "$arg3" ]; then
        split_delim="$arg3"
    else
        split_delim="[、,;| ]+"
    fi
    
    awk -v delim="$split_delim" '{
        # 将整行按分隔符拆分
        gsub(delim, "\n", $0)
        print $0
    }' "$src" | awk '{
        if(NF > 0 && $0 != "") {
            gsub(/^[ \t]+|[ \t]+$/, "", $0)  # 去除首尾空白
            if($0 != "") print $0
        }
    }'
    rm -f "$tmpfile"
    exit 0
fi

# 转为CSV
if [ "$arg2" = "csv" ]; then
    awk -v FS="$awk_delim" -v OFS="," '{print $0}' "$src"
    rm -f "$tmpfile"
    exit 0
fi

# check 功能：检测全文件行列数一致性，输出具体不一致行号和内容
if [ "$arg2" = "check" ]; then
    awk -v FS="$awk_delim" -v sym="$delim" '
    NR==1 {nf=NF; next}
    NF!=nf {
        print "不一致行号:" NR ", 列数:" NF ", 内容: " $0
        bad++
    }
    END{
        if(bad)
            print "共有 " bad " 行列数不同 (分隔符: " sym ")"
        else
            print "所有行列数相同 (分隔符: " sym ")"
    }' "$src"
    rm -f "$tmpfile"
    exit 0
fi

# stats 功能：数据统计分析
if [ "$arg2" = "stats" ]; then
    echo "=== 文件统计信息 ==="
    echo "分隔符: $delim"
    echo "编码: $encoding"
    
    # 获取文件基本信息
    file_size=$(wc -c < "$file" | tr -d ' ')
    if [ "$file_size" -gt 1073741824 ]; then
        size_display=$(awk "BEGIN {printf \"%.2fGB\", $file_size/1073741824}")
    elif [ "$file_size" -gt 1048576 ]; then
        size_display=$(awk "BEGIN {printf \"%.2fMB\", $file_size/1048576}")
    elif [ "$file_size" -gt 1024 ]; then
        size_display=$(awk "BEGIN {printf \"%.2fKB\", $file_size/1024}")
    else
        size_display="${file_size}B"
    fi
    echo "文件大小: $size_display"
    
    awk -v FS="$awk_delim" '
    NR==1 {
        total_cols = NF
        print "总列数: " total_cols
        # 保存列名
        for(i=1; i<=NF; i++) {
            headers[i] = $i
        }
        next
    }
    {
        total_rows++
        # 统计每列的数据
        for(i=1; i<=NF; i++) {
            if($i == "" || $i ~ /^[ \t]*$/) {
                empty_count[i]++
            } else {
                non_empty_count[i]++
                # 检测数据类型
                if($i ~ /^-?[0-9]+$/) {
                    numeric_count[i]++
                } else if($i ~ /^-?[0-9]*\.[0-9]+$/) {
                    float_count[i]++
                } else {
                    text_count[i]++
                }
                # 记录唯一值
                unique_values[i,$i] = 1
            }
        }
        # 检测重复行
        if(seen[$0]++) {
            duplicate_rows++
        }
    }
    END {
        print "总行数: " total_rows " (不含表头)"
        print "重复行数: " (duplicate_rows ? duplicate_rows : 0)
        print ""
        print "=== 各列统计 ==="
        
        for(i=1; i<=total_cols; i++) {
            print "列 " i " (" headers[i] "):"
            printf "  空值: %d (%.1f%%)\n", 
                (empty_count[i] ? empty_count[i] : 0), 
                (empty_count[i] ? empty_count[i]*100/total_rows : 0)
            
            # 统计唯一值数量
            unique_count = 0
            for(key in unique_values) {
                split(key, arr, SUBSEP)
                if(arr[1] == i) unique_count++
            }
            printf "  唯一值: %d\n", unique_count
            
            # 数据类型分析
            total_non_empty = (non_empty_count[i] ? non_empty_count[i] : 0)
            if(total_non_empty > 0) {
                num_count = (numeric_count[i] ? numeric_count[i] : 0)
                float_count_val = (float_count[i] ? float_count[i] : 0)
                text_count_val = (text_count[i] ? text_count[i] : 0)
                
                if(num_count == total_non_empty) {
                    print "  数据类型: 整数"
                } else if((num_count + float_count_val) == total_non_empty) {
                    print "  数据类型: 数值"
                } else if(text_count_val == total_non_empty) {
                    print "  数据类型: 文本"
                } else {
                    print "  数据类型: 混合"
                }
            } else {
                print "  数据类型: 全空"
            }
            print ""
        }
    }' "$src"
    rm -f "$tmpfile"
    exit 0
fi

# dedup 功能：去除重复行
if [ "$arg2" = "dedup" ]; then
    awk -v FS="$awk_delim" '
    NR==1 { print; next }  # 保留表头
    !seen[$0]++           # 只输出未见过的行
    ' "$src"
    rm -f "$tmpfile"
    exit 0
fi

# duplicates 功能：检测和显示重复行
if [ "$arg2" = "duplicates" ]; then
    echo "=== 重复行检测结果 ==="
    echo "分隔符: $delim"
    echo ""
    
    awk -v FS="$awk_delim" '
    NR==1 {
        header = $0
        next
    }
    {
        # 统计每行出现次数
        count[$0]++
        # 记录行号
        if(line_nums[$0] == "") {
            line_nums[$0] = NR
        } else {
            line_nums[$0] = line_nums[$0] "," NR
        }
    }
    END {
        total_duplicates = 0
        duplicate_groups = 0
        
        print "表头: " header
        print ""
        
        # 显示重复行组
        for(line in count) {
            if(count[line] > 1) {
                duplicate_groups++
                total_duplicates += count[line]
                printf "重复组 %d (出现 %d 次):\n", duplicate_groups, count[line]
                printf "行号: %s\n", line_nums[line]
                printf "内容: %s\n", line
                print ""
            }
        }
        
        if(duplicate_groups == 0) {
            print "没有发现重复行"
        } else {
            printf "总结: 共有 %d 个重复组，涉及 %d 行数据\n", duplicate_groups, total_duplicates
            unique_lines = NR - 1 - (total_duplicates - duplicate_groups)
            printf "唯一行数: %d\n", unique_lines
            printf "重复率: %.2f%%\n", (total_duplicates - duplicate_groups) * 100 / (NR - 1)
        }
    }' "$src"
    rm -f "$tmpfile"
    exit 0
fi

# 判断是列号还是列名
if [[ "$arg2" =~ ^[0-9,]+$ ]]; then
    # 按列号提取
    awk -v FS="$awk_delim" -v cols="$arg2" '
    BEGIN{n=split(cols, arr, ",")}
    {
        out=""
        for(i=1;i<=n;i++){
            if(i>1) out=out OFS
            out=out $arr[i]
        }
        print out
    }' "$src"
else
    # 按列名模糊匹配提取
    awk -v FS="$awk_delim" -v cols="$arg2" '
    NR==1{
        n=split(cols, want, ",")
        for(i=1;i<=NF;i++) {
            for(j=1;j<=n;j++) {
                if(tolower($i) ~ tolower(want[j])) idx[j] = i
            }
        }
        next
    }
    {
        out=""
        for(j=1;j<=n;j++) {
            if(j>1) out=out OFS
            out=out $idx[j]
        }
        print out
    }' "$src"
fi

rm -f "$tmpfile"