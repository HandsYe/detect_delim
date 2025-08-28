# detect_delim

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Shell Script](https://img.shields.io/badge/Shell-Bash-green.svg)](https://www.gnu.org/software/bash/)
[![C Language](https://img.shields.io/badge/C-Language-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

🔧 **智能数据处理工具** - 自动检测分隔符，支持数据分析、FASTA处理、字符串拆分

提供Shell和C语言两个版本，适用于生物信息学数据处理、文件格式转换、数据质量控制等场景。

## ✨ 主要功能

- 🔍 **智能检测** - 自动识别分隔符（TAB、逗号、分号等）和文件编码
- 📊 **数据分析** - 统计分析、重复检测、数据提取、格式转换
- 🧬 **FASTA处理** - 序列列表、提取、批量操作、模糊匹配
- 🔤 **字符串拆分** - 智能拆分基因名、样本ID等列表数据
- ⚡ **双版本支持** - Shell版本功能完整，C版本性能优异

## 📦 版本选择

| 特性 | Shell版本 | C语言版本 |
|------|-----------|-----------|
| 性能 | 中等 | 高速 (20-50x) |
| 功能 | 完整 | 核心功能 |
| 依赖 | bash, awk等 | 无依赖 |
| 编码支持 | 多编码自动转换 | UTF-8 |
| 适用场景 | 功能完整性优先 | 性能优先 |

## 🚀 快速开始

### Shell版本（推荐新手）
```bash
# 直接使用
chmod +x detect_delim.sh
./detect_delim.sh data.csv stats
```

### C语言版本（推荐生产环境）
```bash
# 编译
make
# 或 gcc -O2 -o detect_delim detect_delim.c

# 使用
./detect_delim data.csv stats
```

## 📖 基本用法

### 数据检测与分析
```bash
# 检测文件格式
./detect_delim.sh data.csv

# 查看列结构
./detect_delim.sh data.csv head

# 详细统计分析
./detect_delim.sh data.csv stats

# 检查数据完整性
./detect_delim.sh data.csv check
```

### 数据提取与转换
```bash
# 按列号提取
./detect_delim.sh data.csv 1,3,5

# 按列名提取（模糊匹配）
./detect_delim.sh data.csv "gene,expression,pvalue"

# 转换为CSV格式
./detect_delim.sh data.tsv csv

# 去除重复行
./detect_delim.sh data.csv dedup
```

### FASTA序列处理
```bash
# 列出所有序列
./detect_delim.sh sequences.fa fasta list

# 提取单个序列
./detect_delim.sh sequences.fa fasta Stx1

# 批量提取序列
./detect_delim.sh sequences.fa fasta "Stx1,Stx2,LT"

# 保存到文件
./detect_delim.sh sequences.fa fasta Staphylococcus output.fa
```

### 字符串处理
```bash
# 拆分基因名列表
./detect_delim.sh "Stx1、Stx2、LT、ST" split

# 拆分样本ID
./detect_delim.sh "S001,S002;S003|S004" split

# 自定义分隔符
./detect_delim.sh "gene1:gene2:gene3" split ":"
```

## 💡 应用场景

### 🧬 生物信息学
```bash
# 病原体检测引物管理
./detect_delim.sh primers.fa fasta "Staphylococcus_aureus"

# 基因表达数据分析
./detect_delim.sh expression.txt "gene_name,fold_change,pvalue"

# 抗性基因筛选
./detect_delim.sh resistance.fa fasta "mecA,vanA,blaKPC"
```

### 📊 数据质量控制
```bash
# 数据质量评估流程
./detect_delim.sh data.csv stats          # 统计分析
./detect_delim.sh data.csv duplicates     # 重复检测
./detect_delim.sh data.csv dedup > clean.csv  # 数据清理
```

### 🔄 批量处理
```bash
# 批量格式转换
for file in *.tsv; do
    ./detect_delim.sh "$file" csv > "${file%.tsv}.csv"
done

# 批量数据提取
for file in *.csv; do
    ./detect_delim.sh "$file" "1,3,5" > "${file%.csv}_subset.csv"
done
```

## 🛠️ 系统要求

### Shell版本
- **系统**: Linux, macOS, Windows (WSL/Git Bash)
- **依赖**: bash, awk, iconv, file, grep

### C语言版本
- **编译器**: GCC 4.9+, Clang 3.5+, Visual Studio 2015+
- **系统**: 跨平台支持
- **依赖**: 仅需标准C库

## 📊 性能对比

基于1MB测试文件的性能测试：

| 操作 | Shell版本 | C语言版本 | 性能提升 |
|------|-----------|-----------|----------|
| 启动时间 | 200ms | 8ms | 25x |
| 统计分析 | 2.3s | 0.08s | 29x |
| 数据提取 | 1.8s | 0.06s | 30x |
| 格式转换 | 1.5s | 0.05s | 30x |

## 🔧 编译说明

### 快速编译
```bash
# Linux/macOS
make

# Windows (MinGW)
gcc -O2 -o detect_delim.exe detect_delim.c

# 优化编译
gcc -O3 -march=native -o detect_delim detect_delim.c
```

### 编译选项
- **基础版本**: `make`
- **调试版本**: `make debug`
- **静态链接**: `make static`
- **Windows版本**: `make windows`

## 📋 支持格式

### 分隔符类型
- **TAB**: 制表符分隔 (TSV文件)
- **逗号**: 标准CSV格式
- **分号**: 欧洲CSV格式
- **管道符**: 数据库导出格式
- **空格**: 简单文本格式
- **多空格**: 对齐文本格式

### 文件类型
- **表格数据**: CSV, TSV, 自定义分隔符文件
- **FASTA文件**: 标准生物序列格式
- **文本文件**: 任意文本格式

## 🐛 故障排除

### 常见问题
```bash
# 编码问题
file -bi your_file.txt  # 检查编码
iconv -f GBK -t UTF-8 input.txt > output.txt  # 转换编码

# 权限问题
chmod +x detect_delim.sh  # 添加执行权限

# 编译问题
make clean && make  # 清理重新编译
```

### 性能优化
- 大文件先用小样本测试
- 使用SSD存储提高I/O性能
- 批量处理时使用shell循环
- 输出重定向到文件提高效率

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

1. Fork 项目
2. 创建功能分支
3. 提交更改
4. 发起 Pull Request

---

⭐ 如果这个工具对您有帮助，请给我们一个星标！
