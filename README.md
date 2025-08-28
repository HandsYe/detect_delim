# detect_delim

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Shell Script](https://img.shields.io/badge/Shell-Bash-green.svg)](https://www.gnu.org/software/bash/)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-blue.svg)](https://github.com/yourusername/detect_delim)

🔧 **多功能数据处理工具** - 专为生物信息学和数据分析设计的命令行工具

自动检测文件分隔符和编码，支持表格数据分析、FASTA序列处理、字符串拆分等多种功能。无论是处理基因表达数据、管理序列库，还是进行数据质量控制，detect_delim 都能提供高效的解决方案。

## 📑 目录

- [核心功能](#-核心功能)
- [快速开始](#-快速开始)
- [使用指南](#-使用指南)
- [实际应用场景](#-实际应用场景)
- [技术规格](#️-技术规格)
- [工作原理](#️-工作原理)
- [支持格式](#-支持格式)
- [故障排除](#-故障排除)
- [更多资源](#-更多资源)

## ✨ 核心功能

### 🔍 智能数据检测

- **自动分隔符识别** - 支持制表符、逗号、分号、管道符、空格等
- **编码自动转换** - 检测并转换为UTF-8编码
- **数据完整性检查** - 识别格式不一致的行

### 📊 数据分析与处理

- **灵活数据提取** - 按列号或列名（模糊匹配）提取数据
- **格式标准化** - 转换为标准CSV格式
- **统计分析** - 详细的数据质量报告（行列统计、数据类型、空值率等）
- **重复数据管理** - 检测、分析和清理重复数据

### 🧬 生物信息学专用功能

- **FASTA序列处理** - 序列提取、批量处理、模糊匹配
- **基因名称管理** - 智能拆分和整理基因列表
- **引物库管理** - 高效的序列检索和提取
- **病原体数据筛选** - 快速定位目标序列

### 🔤 字符串处理

- **智能拆分** - 支持多种分隔符的混合文本处理
- **自定义分隔符** - 灵活的正则表达式支持
- **批量文本处理** - 高效处理样本ID、基因名等列表数据

## 🚀 快速开始

### 安装

```bash
# 下载脚本
wget https://raw.githubusercontent.com/yourusername/detect_delim/main/detect_delim.sh
chmod +x detect_delim.sh

# 或克隆整个仓库
git clone https://github.com/yourusername/detect_delim.git
cd detect_delim
chmod +x detect_delim.sh
```

### 基本用法

```bash
# 查看完整帮助信息
./detect_delim.sh

# 快速检测文件格式
./detect_delim.sh data.txt

# 数据质量分析
./detect_delim.sh data.csv stats
```

## 📖 使用指南

### 🔍 数据探索与检测

```bash
./detect_delim.sh data.txt                    # 检测分隔符和编码
./detect_delim.sh data.csv head               # 查看列结构
./detect_delim.sh data.csv check              # 检查数据完整性
./detect_delim.sh data.csv stats              # 详细统计分析
```

### 📊 数据提取与转换

```bash
./detect_delim.sh data.csv 1,3,5              # 按列号提取
./detect_delim.sh data.csv "gene,expression"  # 按列名提取（模糊匹配）
./detect_delim.sh data.tsv csv                # 转换为CSV格式
./detect_delim.sh data.csv dedup              # 去除重复行
```

### 🧬 FASTA序列处理

```bash
./detect_delim.sh sequences.fa fasta list                    # 列出所有序列
./detect_delim.sh sequences.fa fasta Stx1                    # 提取单个序列
./detect_delim.sh sequences.fa fasta "Stx1,Stx2,LT"         # 批量提取
./detect_delim.sh sequences.fa fasta Staphylococcus output.fa # 保存到文件
```

### 🔤 字符串处理

```bash
./detect_delim.sh "Stx1、Stx2、LT" split                     # 拆分基因名
./detect_delim.sh "sample1,sample2;sample3" split            # 拆分样本ID
./detect_delim.sh "gene1:gene2:gene3" split ":"              # 自定义分隔符
```

## 💡 实际应用场景

### 🧬 生物信息学工作流

#### 🦠 病原体检测引物管理

```bash
# 管理引物库
./detect_delim.sh primers.fa fasta list                      # 查看所有引物
./detect_delim.sh primers.fa fasta Staphylococcus_aureus     # 提取金黄色葡萄球菌引物
./detect_delim.sh primers.fa fasta "Stx,LT,ST" > toxins.fa  # 提取毒素基因引物
```

#### 🧪 基因表达数据分析

```bash
# 数据质量控制流程
./detect_delim.sh expression.txt stats                       # 数据质量评估
./detect_delim.sh expression.txt duplicates                  # 检查重复样本
./detect_delim.sh expression.txt dedup > clean_data.csv     # 清理数据

# 提取关键信息
./detect_delim.sh expression.txt "gene_name,fold_change,pvalue"
```

#### 🔬 抗性基因分析

```bash
# 整理抗性基因列表
./detect_delim.sh "mecA、vanA、blaKPC、NDM-1、OXA-48" split

# 从序列库提取抗性基因
./detect_delim.sh resistance_db.fa fasta "mecA,vanA,blaKPC" > resistance_genes.fa
```

#### 📋 样本信息管理

```bash
# 批量处理样本ID
./detect_delim.sh "S001,S002,S003;S004|S005 S006" split

# 提取样本关键信息
./detect_delim.sh sample_metadata.txt "sample_id,species,resistance_profile"
```

### 📊 数据分析示例

#### 数据质量报告

```bash
./detect_delim.sh data.csv stats
```

```
=== 文件统计信息 ===
分隔符: ,
编码: utf-8
文件大小: 2.5MB
总列数: 8
总行数: 1,234 (不含表头)
重复行数: 23

=== 各列统计 ===
列 1 (sample_id):
  空值: 0 (0.0%)
  唯一值: 1,234
  数据类型: 文本

列 2 (gene_expression):
  空值: 15 (1.2%)
  唯一值: 892
  数据类型: 数值
```

#### 重复数据分析

```bash
./detect_delim.sh data.csv duplicates
```

```
=== 重复行检测结果 ===
重复组 1 (出现 3 次):
行号: 45,156,789
内容: SAMPLE_001,BRCA1,2.5,0.001

总结: 共有 8 个重复组，涉及 23 行数据
重复率: 1.86%
```

## 🛠️ 技术规格

### 系统要求

- **操作系统**: Linux, macOS, Windows (WSL/Git Bash)
- **Shell**: Bash 4.0+
- **依赖工具**: `awk`, `iconv`, `file`, `grep` (通常系统自带)

### 性能特点

- **内存效率**: 流式处理，支持大文件
- **处理速度**: 优化的awk脚本，快速数据处理
- **编码兼容**: 自动处理多种字符编码
- **错误处理**: 友好的错误提示和恢复机制

## ⚙️ 工作原理

### 智能检测算法

1. **编码识别** - 使用 `file`命令检测字符编码，自动转换为UTF-8
2. **分隔符分析** - 统计首行各分隔符频率，智能判断最可能的分隔符
3. **结构验证** - 逐行检查列数一致性，识别格式异常
4. **模糊匹配** - 基于正则表达式的灵活列名和序列名匹配

### 数据处理流程

```
输入文件 → 编码检测 → 分隔符识别 → 数据解析 → 功能处理 → 结果输出
    ↓           ↓           ↓           ↓           ↓           ↓
  原始数据   UTF-8转换   格式标准化   结构化数据   业务逻辑   格式化输出
```

## 📋 支持格式

### 🔍 自动检测的分隔符

| 分隔符         | 描述   | 常见用途            |
| -------------- | ------ | ------------------- |
| `TAB`        | 制表符 | TSV文件、数据库导出 |
| `,`          | 逗号   | 标准CSV文件         |
| `;`          | 分号   | 欧洲CSV格式         |
| `              | `      | 管道符              |
|                | 空格   | 简单文本格式        |
| `MULTISPACE` | 多空格 | 对齐格式的文本      |

### 🧬 FASTA文件支持

- 标准FASTA格式 (`>header` + 序列)
- 多行序列支持
- 模糊序列名匹配
- 批量序列提取

### 🔤 字符串拆分分隔符

- **默认支持**: `、` `,` `;` `|`  (空格)
- **自定义**: 支持正则表达式分隔符
- **混合分隔符**: 一次处理多种分隔符混合的文本

## 🔧 故障排除

### 常见问题

#### 编码问题

```bash
# 问题：乱码或编码错误
# 解决：检查原文件编码
file -bi your_file.txt

# 手动转换编码
iconv -f GBK -t UTF-8 your_file.txt > converted_file.txt
```

#### 分隔符检测不准确

```bash
# 问题：分隔符识别错误
# 解决：检查文件首行是否具有代表性
head -1 your_file.txt

# 手动指定处理（通过转换为标准格式）
./detect_delim.sh your_file.txt csv > standard_format.csv
```

#### 权限问题

```bash
# 问题：权限不足
# 解决：检查脚本执行权限
chmod +x detect_delim.sh

# 检查文件读取权限
ls -la your_file.txt
```

### 性能优化建议

- **大文件处理**: 先用小样本测试，确认格式正确
- **批量处理**: 使用shell循环处理多个文件
- **内存优化**: 对于超大文件，考虑分块处理
- **输出重定向**: 将结果保存到文件以提高效率

```bash
# 批量处理示例
for file in *.csv; do
    ./detect_delim.sh "$file" stats > "${file%.csv}_stats.txt"
done
```

## 📚 更多资源

### 📖 详细文档

- [完整使用指南](USAGE.md) - 详细的功能说明和应用场景
- [API参考](docs/API.md) - 所有参数和选项的详细说明
- [最佳实践](docs/best-practices.md) - 生物信息学数据处理建议

### 🤝 贡献指南

欢迎提交Issue和Pull Request！

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启Pull Request

### 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情

---

⭐ 如果这个工具对您有帮助，请给我们一个星标！
