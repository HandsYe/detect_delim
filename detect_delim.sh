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