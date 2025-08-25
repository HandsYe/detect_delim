# detect_delim

一个强大的命令行工具，用于自动检测文件分隔符和编码，支持列提取、格式转换、文件结构检测和字符串处理等功能。

## 功能特性

### 📁 文件处理功能
- 自动检测文件分隔符（支持 TAB、逗号、分号、管道符、空格等）
- 自动检测文件编码并转换为 UTF-8
- 支持按列号提取数据
- 支持按列名模糊匹配提取数据（不区分大小写）
- 转换为标准 CSV 格式
- 检测文件行列数一致性
- 显示文件列名和对应列号
- 文件内容按指定分隔符拆分
- 数据统计分析（行数、列数、数据类型、空值、重复值等）
- 去除重复行功能
- 检测和显示重复行（详细分析重复的内容和位置）

### 🔤 字符串处理功能
- 字符串按分隔符拆分
- 支持自定义分隔符或使用默认分隔符组合

## 使用方法

### 基本语法
```bash
# 文件处理
./detect_delim.sh <文件路径> [选项]

# 字符串处理
./detect_delim.sh "字符串" split [分隔符]
```

### 文件操作命令
```bash
# 检测文件分隔符
./detect_delim.sh <文件路径>

# 提取指定列（按列号）
./detect_delim.sh <文件路径> <列号,...>

# 提取指定列（按列名，支持模糊匹配）
./detect_delim.sh <文件路径> <列名,...>

# 转换为 CSV 格式
./detect_delim.sh <文件路径> csv

# 检测文件行列数一致性
./detect_delim.sh <文件路径> check

# 显示列名和对应列号
./detect_delim.sh <文件路径> head

# 数据统计分析
./detect_delim.sh <文件路径> stats

# 去除重复行
./detect_delim.sh <文件路径> dedup

# 检测和显示重复行
./detect_delim.sh <文件路径> duplicates

# 文件内容按分隔符拆分
./detect_delim.sh <文件路径> split [分隔符]
```

### 字符串操作命令
```bash
# 字符串按默认分隔符拆分
./detect_delim.sh "字符串" split

# 字符串按指定分隔符拆分
./detect_delim.sh "字符串" split "[分隔符]"
```

## 使用示例

### 文件处理示例
```bash
# 检测 sample.txt 文件的分隔符
./detect_delim.sh sample.txt
# 输出: TAB (或 , ; | 等)

# 提取第1列和第3列
./detect_delim.sh data.csv "1,3"

# 提取包含"name"和"age"的列（不区分大小写模糊匹配）
./detect_delim.sh data.csv "name,age"

# 转换为 CSV 格式
./detect_delim.sh data.tsv csv

# 检查文件结构一致性
./detect_delim.sh data.csv check
# 输出: 不一致行号:3, 列数:2, 内容: John,Smith
#       共有 1 行列数不同 (分隔符: ,)

# 显示列名和对应列号
./detect_delim.sh data.csv head
# 输出: 列名和对应的列号:
#       1: Name
#       2: Age
#       3: City

# 数据统计分析
./detect_delim.sh data.csv stats
# 输出: === 文件统计信息 ===
#       分隔符: ,
#       编码: utf-8
#       文件大小: 224B
#       总列数: 4
#       总行数: 8 (不含表头)
#       重复行数: 2
#       === 各列统计 ===
#       列 1 (Name):
#         空值: 0 (0.0%)
#         唯一值: 6
#         数据类型: 文本

# 去除重复行
./detect_delim.sh data.csv dedup
# 输出: 去重后的CSV数据（保留表头，移除重复行）

# 检测和显示重复行
./detect_delim.sh data.csv duplicates
# 输出: === 重复行检测结果 ===
#       分隔符: ,
#       表头: Name,Age,City,Salary
#       重复组 1 (出现 3 次):
#       行号: 2,5,8
#       内容: 张三,25,北京,8000
#       总结: 共有 1 个重复组，涉及 3 行数据
#       唯一行数: 6
#       重复率: 25.00%

# 文件内容按逗号拆分
./detect_delim.sh data.txt split ","
```

### 字符串处理示例
```bash
# 使用默认分隔符拆分字符串
./detect_delim.sh "apple、banana,orange;grape|mango" split
# 输出: apple
#       banana
#       orange
#       grape
#       mango

# 使用自定义分隔符拆分
./detect_delim.sh "name=John;age=25;city=NYC" split "[=;]"
# 输出: name
#       John
#       age
#       25
#       city
#       NYC

# 按空格拆分句子
./detect_delim.sh "Hello world from detect_delim" split " "
# 输出: Hello
#       world
#       from
#       detect_delim
```

## 依赖

- `bash`
- `awk`
- `iconv`
- `file`
- `grep`

## 工作原理

1. **编码检测**：使用 `file` 命令检测文件编码，如果不是 UTF-8，则使用 `iconv` 转换为 UTF-8
2. **分隔符检测**：通过分析文件第一行中各种分隔符出现的频率来判断文件分隔符
3. **数据处理**：使用 `awk` 进行列提取、格式转换等操作
4. **一致性检查**：逐行检查每行的列数是否一致

## 支持的分隔符

### 文件分隔符检测
- **TAB**: 制表符分隔
- **逗号 (,)**: 标准CSV格式
- **分号 (;)**: 欧洲地区常用格式
- **管道符 (|)**: 数据库导出常用格式
- **空格 ( )**: 单个空格分隔
- **MULTISPACE**: 多个连续空格分隔

### 字符串拆分默认分隔符
- **中文逗号 (、)**
- **英文逗号 (,)**
- **分号 (;)**
- **竖线 (|)**
- **空格 ( )**

> 字符串拆分支持正则表达式格式的自定义分隔符

## 安装和运行

### 安装
```bash
# 克隆或下载脚本文件
chmod +x detect_delim.sh
```

### 系统要求
- **类Unix系统**: Linux、macOS、WSL等
- **Windows**: 需要安装Git Bash、WSL或类似环境

## 注意事项

### ⚠️ 重要提醒
- 该工具会创建临时文件用于编码转换，请确保有写入权限
- 列名匹配是模糊匹配且不区分大小写
- 分隔符检测基于文件第一行，请确保第一行具有代表性
- 对于结构不一致的文件，建议先使用 `check` 功能检查

### 💡 使用技巧
- 使用 `head` 命令先查看列结构，再进行列提取
- 使用 `stats` 功能全面了解数据的质量和结构
- 使用 `duplicates` 功能详细分析重复数据的分布和位置
- 使用 `dedup` 功能清理重复数据，提高数据质量
- 对于大文件，建议先用小样本测试分隔符检测效果
- 字符串拆分功能可以处理各种混合分隔符的文本
- `check` 功能会显示具体的不一致行，便于数据清理
- 可以将 `dedup` 输出重定向到文件：`./detect_delim.sh data.csv dedup > clean_data.csv`
- 数据清理流程建议：`stats` → `duplicates` → `dedup` → `check`

### 🔧 故障排除
- 如果编码检测失败，请检查文件是否损坏
- 如果分隔符检测不准确，可能是文件格式特殊，建议手动指定
- 权限不足时，检查脚本文件和目标目录的读写权限