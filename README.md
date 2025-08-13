# detect_delim

自动检测文件分隔符和编码的工具，支持列提取、转CSV、文件结构检测等功能。

## 功能特性

- 自动检测文件分隔符（支持 TAB、逗号、分号、管道符、空格等）
- 自动检测文件编码并转换为 UTF-8
- 支持按列号提取数据
- 支持按列名模糊匹配提取数据
- 支持转换为 CSV 格式
- 检测文件行列数一致性

## 使用方法

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
```

## 使用示例

```bash
# 检测 sample.txt 文件的分隔符
./detect_delim.sh sample.txt

# 提取第1列和第3列
./detect_delim.sh data.csv "1,3"

# 提取包含"name"和"age"的列（不区分大小写模糊匹配）
./detect_delim.sh data.csv "name,age"

# 转换为 CSV 格式
./detect_delim.sh data.tsv csv

# 检查文件结构一致性
./detect_delim.sh data.csv check
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

- TAB
- 逗号 (,)
- 分号 (;)
- 管道符 (|)
- 空格 ( )
- 多空格 (MULTISPACE)

## 注意事项

- 该工具会创建临时文件用于编码转换，请确保有写入权限
- 列名匹配是模糊匹配且不区分大小写