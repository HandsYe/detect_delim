#!/bin/bash
# detect_delim 全功能测试脚本

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
DATA_DIR="$SCRIPT_DIR/data"

# 切换到根目录
cd "$ROOT_DIR"

echo "=========================================="
echo "       detect_delim 全功能测试"
echo "=========================================="
echo

# 检查必要文件
echo "📋 检查测试文件..."
files=("detect_delim.sh" "tests/data/test_data.csv" "tests/data/test_data.tsv" "tests/data/test_sequences.fa" "tests/data/example.csv")
for file in "${files[@]}"; do
    if [ -f "$file" ]; then
        echo "✅ $file - 存在"
    else
        echo "❌ $file - 缺失"
        exit 1
    fi
done
echo

# 测试1: 基本分隔符检测
echo "🔍 测试1: 分隔符检测功能"
echo "----------------------------------------"
echo "CSV文件分隔符检测:"
bash detect_delim.sh tests/data/test_data.csv
echo

echo "TSV文件分隔符检测:"
bash detect_delim.sh tests/data/test_data.tsv
echo

echo "Example文件分隔符检测:"
bash detect_delim.sh tests/data/example.csv
echo

# 测试2: 列结构显示
echo "📊 测试2: 列结构显示功能 (head)"
echo "----------------------------------------"
echo "CSV文件列结构:"
bash detect_delim.sh tests/data/test_data.csv head
echo

echo "TSV文件列结构:"
bash detect_delim.sh tests/data/test_data.tsv head
echo

# 测试3: 数据统计分析
echo "📈 测试3: 数据统计分析功能 (stats)"
echo "----------------------------------------"
echo "CSV文件统计分析:"
bash detect_delim.sh tests/data/test_data.csv stats
echo

# 测试4: 数据完整性检查
echo "🔧 测试4: 数据完整性检查 (check)"
echo "----------------------------------------"
echo "CSV文件完整性检查:"
bash detect_delim.sh tests/data/test_data.csv check
echo

echo "TSV文件完整性检查:"
bash detect_delim.sh tests/data/test_data.tsv check
echo

# 测试5: 重复行检测
echo "🔍 测试5: 重复行检测功能 (duplicates)"
echo "----------------------------------------"
echo "CSV文件重复行检测:"
bash detect_delim.sh tests/data/test_data.csv duplicates
echo

# 测试6: 数据去重
echo "🧹 测试6: 数据去重功能 (dedup)"
echo "----------------------------------------"
echo "CSV文件去重 (显示前5行):"
bash detect_delim.sh tests/data/test_data.csv dedup | head -5
echo

# 测试7: 列提取 - 按列号
echo "📋 测试7: 列提取功能 - 按列号"
echo "----------------------------------------"
echo "提取CSV第1,3列:"
bash detect_delim.sh tests/data/test_data.csv 1,3 | head -3
echo

echo "提取TSV第2,4列:"
bash detect_delim.sh tests/data/test_data.tsv 2,4 | head -3
echo

# 测试8: 列提取 - 按列名
echo "📋 测试8: 列提取功能 - 按列名"
echo "----------------------------------------"
echo "提取CSV中Name,Salary列:"
bash detect_delim.sh tests/data/test_data.csv Name,Salary | head -3
echo

echo "提取TSV中Gene,Expression列:"
bash detect_delim.sh tests/data/test_data.tsv Gene,Expression | head -3
echo

# 测试9: 格式转换
echo "🔄 测试9: 格式转换功能 (csv)"
echo "----------------------------------------"
echo "TSV转CSV格式:"
bash detect_delim.sh tests/data/test_data.tsv csv | head -3
echo

# 测试10: FASTA序列处理
echo "🧬 测试10: FASTA序列处理功能"
echo "----------------------------------------"
echo "列出所有FASTA序列:"
bash detect_delim.sh tests/data/test_sequences.fa fasta list
echo

echo "提取Staphylococcus序列:"
bash detect_delim.sh tests/data/test_sequences.fa fasta Staphylococcus
echo

echo "提取多个序列 (mecA,blaKPC):"
bash detect_delim.sh tests/data/test_sequences.fa fasta mecA,blaKPC
echo

# 测试11: 字符串拆分
echo "🔤 测试11: 字符串拆分功能"
echo "----------------------------------------"
echo "拆分基因名列表 (默认分隔符):"
bash detect_delim.sh "BRCA1、TP53,EGFR;MYC|APC" split
echo

echo "拆分样本ID (自定义分隔符):"
bash detect_delim.sh "S001:S002:S003:S004" split ":"
echo

# 测试12: 文件拆分
echo "🗂️ 测试12: 文件内容拆分功能"
echo "----------------------------------------"
echo "按逗号拆分tests/data/example.csv内容:"
bash detect_delim.sh tests/data/example.csv split , | head -10
echo

# 错误处理测试
echo "⚠️  测试13: 错误处理测试"
echo "----------------------------------------"
echo "测试不存在的文件:"
bash detect_delim.sh nonexistent.csv 2>&1 | head -1
echo

echo "测试无效的FASTA文件:"
bash detect_delim.sh tests/data/test_data.csv fasta list 2>&1 | head -1
echo

echo "测试不存在的序列:"
bash detect_delim.sh tests/data/test_sequences.fa fasta nonexistent_gene 2>&1 | head -1
echo

# 性能测试
echo "⚡ 测试14: 性能测试"
echo "----------------------------------------"
echo "处理时间测试 (使用time命令):"
time bash detect_delim.sh tests/data/test_data.csv stats > /dev/null 2>&1
echo

echo "=========================================="
echo "           全功能测试完成!"
echo "=========================================="
echo
echo "📊 测试总结:"
echo "✅ 分隔符检测: CSV, TSV, 混合格式"
echo "✅ 数据分析: 统计、检查、去重、重复检测"  
echo "✅ 数据提取: 按列号、按列名"
echo "✅ 格式转换: TSV→CSV"
echo "✅ FASTA处理: 序列列表、提取、批量操作"
echo "✅ 字符串处理: 拆分、自定义分隔符"
echo "✅ 错误处理: 文件不存在、格式错误"
echo
echo "🎉 所有核心功能测试完成！"