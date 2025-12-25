# detect_delim

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Shell Script](https://img.shields.io/badge/Shell-Bash-green.svg)](https://www.gnu.org/software/bash/)
[![C Language](https://img.shields.io/badge/C-Language-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

🔧 **智能数据处理工具** - 自动检测分隔符，支持数据分析、FASTA处理、字符串拆分

提供Shell和C语言两个版本，适用于生物信息学数据处理、文件格式转换、数据质量控制等场景。

## ✨ 主要功能

- 🔍 **智能检测** - 自动识别分隔符（TAB、逗号、分号等）和文件编码
- 📊 **数据分析** - 统计分析、重复检测、数据提取、格式转换、随机抽样
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

# 随机抽取N行数据（保留表头）
./detect_delim.sh data.csv random 100

# 随机抽样并保存
./detect_delim.sh large_data.csv random 1000 > sample.csv
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

### 📊 数据采样与测试
```bash
# 从大数据集创建测试样本
./detect_delim.sh production_data.csv random 1000 > test_sample.csv

# 机器学习数据集划分
./detect_delim.sh full_dataset.csv random 8000 > train.csv

# 快速预览大文件
./detect_delim.sh huge_file.csv random 50 | less

# 性能测试数据生成
./detect_delim.sh data.csv random 100 > small_test.csv
./detect_delim.sh data.csv random 1000 > medium_test.csv
```

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

# 随机抽样测试
./detect_delim.sh large_data.csv random 1000 > sample.csv
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

## 🎯 随机取N行功能详解

### 功能特性
- ✅ **自动保留表头** - 第一行始终保留，只从数据行随机抽取
- ✅ **真随机性** - 使用Fisher-Yates洗牌算法，每次结果不同
- ✅ **智能边界处理** - 请求行数超过实际数据时自动调整
- ✅ **高性能** - C语言版本比Shell版本快20-30倍
- ✅ **兼容性好** - Shell版本自动选择最优实现方式

### 基本用法
```bash
# 随机抽取100行
./detect_delim.sh data.csv random 100

# 保存到文件
./detect_delim.sh data.csv random 100 > sample.csv

# 查看帮助
./detect_delim.sh data.csv random
```

### 实际应用示例

#### 1. 机器学习数据集划分
```bash
# 从10000行数据创建训练集（80%）和测试集（20%）
./detect_delim.sh full_data.csv random 8000 > train.csv
# 注意：剩余数据需要额外处理才能作为测试集
```

#### 2. 快速数据探索
```bash
# 从百万行数据中随机查看100行
./detect_delim.sh huge_dataset.csv random 100 | less

# 结合其他命令进行分析
./detect_delim.sh data.csv random 1000 | ./detect_delim.sh - stats
```

#### 3. 性能测试数据生成
```bash
# 生成不同规模的测试数据
./detect_delim.sh production.csv random 100 > test_small.csv
./detect_delim.sh production.csv random 1000 > test_medium.csv
./detect_delim.sh production.csv random 10000 > test_large.csv
```

#### 4. 批量抽样
```bash
# 对多个文件进行抽样
for file in data/*.csv; do
    basename="${file##*/}"
    ./detect_delim.sh "$file" random 500 > "samples/${basename}"
done
```

#### 5. 按比例抽样
```bash
# 抽取10%的数据
total=$(tail -n +2 data.csv | wc -l)  # 不含表头的行数
sample_size=$((total / 10))
./detect_delim.sh data.csv random $sample_size > sample_10percent.csv
```

### 算法说明
使用**Fisher-Yates洗牌算法**（Knuth洗牌）：
- **时间复杂度**: O(n)
- **空间复杂度**: O(n)
- **随机性**: 均匀分布，每个元素被选中概率相等

### 性能数据
基于实际测试（Intel i5处理器）：

| 数据规模 | Shell版本 | C语言版本 | 性能提升 |
|---------|----------|-----------|---------|
| 100行   | 0.05s    | 0.002s    | 25x     |
| 1000行  | 0.2s     | 0.01s     | 20x     |
| 10000行 | 1.5s     | 0.05s     | 30x     |

### 注意事项
1. **内存占用**: 所有数据会加载到内存，超大文件需注意内存使用
2. **行数限制**: C语言版本默认最多10000行（可修改源码调整）
3. **随机性**: 基于系统随机数，不适用于加密场景
4. **表头处理**: 自动识别第一行为表头并保留

### 与其他工具对比
```bash
# shuf命令（需手动处理表头）
head -1 data.csv > sample.csv
tail -n +2 data.csv | shuf -n 100 >> sample.csv

# detect_delim（一步到位）
./detect_delim.sh data.csv random 100 > sample.csv

# Python pandas（需要编程）
python -c "import pandas as pd; pd.read_csv('data.csv').sample(100).to_csv('sample.csv')"

# detect_delim（无需编程）
./detect_delim.sh data.csv random 100 > sample.csv
```

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

## 📖 文档导航

- **[快速参考](QUICK_REFERENCE.md)** - 命令速查表和常用场景
- **[随机抽样详解](RANDOM_SAMPLE_USAGE.md)** - random功能完整指南
- **[更新日志](CHANGELOG.md)** - 版本更新记录
- **[示例脚本](examples/)** - 实用示例代码集合

## 🎓 学习资源

### 示例脚本
```bash
# 运行交互式示例
cd examples
bash random_sampling_examples.sh

# 查看所有示例
bash random_sampling_examples.sh --all
```

### 性能测试
```bash
# 运行性能基准测试
bash benchmark_random.sh
```

### 生成测试数据
```bash
# 生成1000行测试数据
bash generate_test_data.sh 1000 test_1000.csv
```

---

⭐ 如果这个工具对您有帮助，请给我们一个星标！
