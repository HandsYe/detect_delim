#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 65536
#define MAX_COLUMNS 1000
#define MAX_FILENAME 256
#define MAX_SEQUENCES 10000

// 分隔符类型枚举
typedef enum {
    DELIM_TAB,
    DELIM_COMMA,
    DELIM_SEMICOLON,
    DELIM_PIPE,
    DELIM_SPACE,
    DELIM_MULTISPACE,
    DELIM_UNKNOWN
} delimiter_type_t;

// 数据类型枚举
typedef enum {
    DATA_INTEGER,
    DATA_FLOAT,
    DATA_TEXT,
    DATA_MIXED,
    DATA_EMPTY
} data_type_t;

// 列统计结构
typedef struct {
    char name[256];
    int empty_count;
    int non_empty_count;
    int unique_count;
    data_type_t data_type;
    int numeric_count;
    int float_count;
    int text_count;
} column_stats_t;

// 文件统计结构
typedef struct {
    delimiter_type_t delimiter;
    char delimiter_char;
    int total_rows;
    int total_columns;
    int duplicate_rows;
    long file_size;
    column_stats_t columns[MAX_COLUMNS];
} file_stats_t;

// FASTA序列结构
typedef struct {
    char header[512];
    char sequence[MAX_LINE_LENGTH];
} fasta_sequence_t;

// 函数声明
void show_usage(const char* program_name);
delimiter_type_t detect_delimiter(const char* filename, char* delim_char);
void analyze_file_stats(const char* filename, file_stats_t* stats);
void extract_columns_by_number(const char* filename, const char* columns);
void extract_columns_by_name(const char* filename, const char* columns);
void convert_to_csv(const char* filename);
void check_file_consistency(const char* filename);
void show_column_headers(const char* filename);
void remove_duplicates(const char* filename);
void show_duplicates(const char* filename);
void split_string(const char* input, const char* delimiter);
void split_file_content(const char* filename, const char* delimiter);
void process_fasta_list(const char* filename);
void process_fasta_extract(const char* filename, const char* sequence_names, const char* output_file);
int is_fasta_file(const char* filename);
data_type_t detect_data_type(const char* value);
void trim_whitespace(char* str);
int count_char_occurrences(const char* str, char ch);
void format_file_size(long size, char* buffer);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        show_usage(argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    const char* operation = (argc > 2) ? argv[2] : NULL;
    const char* param3 = (argc > 3) ? argv[3] : NULL;

    // 字符串拆分功能
    if (operation && strcmp(operation, "split") == 0) {
        if (access(filename, F_OK) != 0) {
            // 文件不存在，作为字符串处理
            split_string(filename, param3);
            return 0;
        } else {
            // 文件存在，拆分文件内容
            split_file_content(filename, param3);
            return 0;
        }
    }

    // FASTA文件处理
    if (operation && strcmp(operation, "fasta") == 0) {
        if (!is_fasta_file(filename)) {
            fprintf(stderr, "错误: 不是有效的FASTA格式文件\n");
            return 1;
        }
        
        if (param3 && strcmp(param3, "list") == 0) {
            process_fasta_list(filename);
        } else if (param3) {
            const char* output_file = (argc > 4) ? argv[4] : NULL;
            process_fasta_extract(filename, param3, output_file);
        } else {
            fprintf(stderr, "请指定要提取的序列名或使用 'list' 查看所有序列\n");
            return 1;
        }
        return 0;
    }

    // 检查文件是否存在
    if (access(filename, F_OK) != 0) {
        fprintf(stderr, "文件不存在: %s\n", filename);
        return 1;
    }

    // 基础功能处理
    if (!operation) {
        // 仅检测分隔符
        char delim_char;
        delimiter_type_t delim = detect_delimiter(filename, &delim_char);
        switch (delim) {
            case DELIM_TAB: printf("TAB\n"); break;
            case DELIM_COMMA: printf(",\n"); break;
            case DELIM_SEMICOLON: printf(";\n"); break;
            case DELIM_PIPE: printf("|\n"); break;
            case DELIM_SPACE: printf(" \n"); break;
            case DELIM_MULTISPACE: printf("MULTISPACE\n"); break;
            default: printf("UNKNOWN\n"); break;
        }
    } else if (strcmp(operation, "head") == 0) {
        show_column_headers(filename);
    } else if (strcmp(operation, "check") == 0) {
        check_file_consistency(filename);
    } else if (strcmp(operation, "stats") == 0) {
        file_stats_t stats;
        analyze_file_stats(filename, &stats);
    } else if (strcmp(operation, "csv") == 0) {
        convert_to_csv(filename);
    } else if (strcmp(operation, "dedup") == 0) {
        remove_duplicates(filename);
    } else if (strcmp(operation, "duplicates") == 0) {
        show_duplicates(filename);
    } else if (strstr(operation, ",") != NULL && strspn(operation, "0123456789,") == strlen(operation)) {
        // 按列号提取
        extract_columns_by_number(filename, operation);
    } else {
        // 按列名提取
        extract_columns_by_name(filename, operation);
    }

    return 0;
}

void show_usage(const char* program_name) {
    printf("detect_delim - 多功能数据处理工具 (C语言版)\n");
    printf("自动检测分隔符 & 编码，支持表格数据分析、FASTA序列处理、字符串拆分等功能\n\n");
    
    printf("=== 基础功能 ===\n");
    printf("  %s <文件路径>                    # 检测文件分隔符\n", program_name);
    printf("  %s <文件路径> head               # 显示列名和对应列号\n", program_name);
    printf("  %s <文件路径> check              # 检查数据完整性\n", program_name);
    printf("\n");
    
    printf("=== 数据提取与转换 ===\n");
    printf("  %s <文件路径> <列号,...>         # 按列号提取数据\n", program_name);
    printf("  %s <文件路径> <列名,...>         # 按列名模糊匹配提取\n", program_name);
    printf("  %s <文件路径> csv               # 转换为标准CSV格式\n", program_name);
    printf("\n");
    
    printf("=== 数据分析 ===\n");
    printf("  %s <文件路径> stats             # 详细统计分析\n", program_name);
    printf("  %s <文件路径> duplicates        # 检测并显示重复行详情\n", program_name);
    printf("  %s <文件路径> dedup             # 去除重复行\n", program_name);
    printf("\n");
    
    printf("=== 字符串处理 ===\n");
    printf("  %s \"字符串\" split [分隔符]      # 拆分字符串并换行显示\n", program_name);
    printf("  %s <文件路径> split [分隔符]     # 拆分文件内容\n", program_name);
    printf("    支持分隔符: 中文逗号(、) 英文逗号(,) 分号(;) 竖线(|) 空格\n");
    printf("\n");
    
    printf("=== FASTA序列处理 ===\n");
    printf("  %s <fasta文件> fasta list                    # 列出所有序列名称\n", program_name);
    printf("  %s <fasta文件> fasta <序列名>                # 提取单个序列\n", program_name);
    printf("  %s <fasta文件> fasta <序列名1,序列名2,...>   # 批量提取序列\n", program_name);
    printf("  %s <fasta文件> fasta <序列名> [输出文件]     # 提取序列并保存\n", program_name);
    printf("    支持模糊匹配: 如 'Stx' 可匹配 'Stx1', 'Stx2' 等\n");
    printf("\n");
    
    printf("=== 使用示例 ===\n");
    printf("  %s data.txt                      # 检测分隔符\n", program_name);
    printf("  %s data.txt stats               # 数据统计分析\n", program_name);
    printf("  %s \"Stx1、Stx2、LT\" split       # 拆分毒素基因名\n", program_name);
    printf("  %s sequences.fa fasta Stx       # 提取Stx相关序列\n", program_name);
}

delimiter_type_t detect_delimiter(const char* filename, char* delim_char) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return DELIM_UNKNOWN;
    }

    char line[MAX_LINE_LENGTH];
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return DELIM_UNKNOWN;
    }
    fclose(file);

    // 统计各种分隔符的出现次数
    int tab_count = count_char_occurrences(line, '\t');
    int comma_count = count_char_occurrences(line, ',');
    int semicolon_count = count_char_occurrences(line, ';');
    int pipe_count = count_char_occurrences(line, '|');
    int space_count = count_char_occurrences(line, ' ');

    // 检查是否有多个连续空格
    int multispace = (strstr(line, "  ") != NULL) ? 1 : 0;

    // 确定最可能的分隔符
    int max_count = 0;
    delimiter_type_t result = DELIM_UNKNOWN;

    if (tab_count > max_count) {
        max_count = tab_count;
        result = DELIM_TAB;
        *delim_char = '\t';
    }
    if (comma_count > max_count) {
        max_count = comma_count;
        result = DELIM_COMMA;
        *delim_char = ',';
    }
    if (semicolon_count > max_count) {
        max_count = semicolon_count;
        result = DELIM_SEMICOLON;
        *delim_char = ';';
    }
    if (pipe_count > max_count) {
        max_count = pipe_count;
        result = DELIM_PIPE;
        *delim_char = '|';
    }
    if (space_count > max_count && !multispace) {
        max_count = space_count;
        result = DELIM_SPACE;
        *delim_char = ' ';
    }
    if (multispace && space_count > 0) {
        result = DELIM_MULTISPACE;
        *delim_char = ' ';
    }

    return result;
}

void analyze_file_stats(const char* filename, file_stats_t* stats) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    stats->file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 检测分隔符
    stats->delimiter = detect_delimiter(filename, &stats->delimiter_char);

    char line[MAX_LINE_LENGTH];
    char header_line[MAX_LINE_LENGTH];
    int line_number = 0;
    int expected_columns = 0;

    printf("=== 文件统计信息 ===\n");
    
    // 显示分隔符
    switch (stats->delimiter) {
        case DELIM_TAB: printf("分隔符: TAB\n"); break;
        case DELIM_COMMA: printf("分隔符: ,\n"); break;
        case DELIM_SEMICOLON: printf("分隔符: ;\n"); break;
        case DELIM_PIPE: printf("分隔符: |\n"); break;
        case DELIM_SPACE: printf("分隔符: 空格\n"); break;
        case DELIM_MULTISPACE: printf("分隔符: 多空格\n"); break;
        default: printf("分隔符: 未知\n"); break;
    }

    // 显示文件大小
    char size_str[64];
    format_file_size(stats->file_size, size_str);
    printf("文件大小: %s\n", size_str);

    // 读取并分析数据
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        
        // 移除换行符
        line[strcspn(line, "\r\n")] = 0;
        
        if (line_number == 1) {
            // 保存表头
            strcpy(header_line, line);
            
            // 计算列数
            char* token = strtok(line, &stats->delimiter_char);
            while (token != NULL) {
                expected_columns++;
                if (expected_columns <= MAX_COLUMNS) {
                    strncpy(stats->columns[expected_columns-1].name, token, 255);
                    stats->columns[expected_columns-1].name[255] = '\0';
                }
                token = strtok(NULL, &stats->delimiter_char);
            }
            stats->total_columns = expected_columns;
            printf("总列数: %d\n", expected_columns);
            continue;
        }

        stats->total_rows++;
        
        // 分析每列的数据
        char line_copy[MAX_LINE_LENGTH];
        strcpy(line_copy, line);
        
        char* token = strtok(line_copy, &stats->delimiter_char);
        int col_index = 0;
        
        while (token != NULL && col_index < expected_columns) {
            trim_whitespace(token);
            
            if (strlen(token) == 0) {
                stats->columns[col_index].empty_count++;
            } else {
                stats->columns[col_index].non_empty_count++;
                
                data_type_t type = detect_data_type(token);
                switch (type) {
                    case DATA_INTEGER:
                        stats->columns[col_index].numeric_count++;
                        break;
                    case DATA_FLOAT:
                        stats->columns[col_index].float_count++;
                        break;
                    case DATA_TEXT:
                        stats->columns[col_index].text_count++;
                        break;
                    default:
                        break;
                }
            }
            
            token = strtok(NULL, &stats->delimiter_char);
            col_index++;
        }
    }

    printf("总行数: %d (不含表头)\n", stats->total_rows);
    printf("\n=== 各列统计 ===\n");

    // 显示每列的统计信息
    for (int i = 0; i < stats->total_columns && i < MAX_COLUMNS; i++) {
        printf("列 %d (%s):\n", i+1, stats->columns[i].name);
        
        float empty_percent = stats->total_rows > 0 ? 
            (float)stats->columns[i].empty_count * 100.0 / stats->total_rows : 0.0;
        printf("  空值: %d (%.1f%%)\n", stats->columns[i].empty_count, empty_percent);
        
        // 确定数据类型
        int total_non_empty = stats->columns[i].non_empty_count;
        if (total_non_empty > 0) {
            if (stats->columns[i].numeric_count == total_non_empty) {
                printf("  数据类型: 整数\n");
            } else if ((stats->columns[i].numeric_count + stats->columns[i].float_count) == total_non_empty) {
                printf("  数据类型: 数值\n");
            } else if (stats->columns[i].text_count == total_non_empty) {
                printf("  数据类型: 文本\n");
            } else {
                printf("  数据类型: 混合\n");
            }
        } else {
            printf("  数据类型: 全空\n");
        }
        printf("\n");
    }

    fclose(file);
}

void extract_columns_by_number(const char* filename, const char* columns) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    char delim_char;
    detect_delimiter(filename, &delim_char);

    // 解析列号
    int col_indices[MAX_COLUMNS];
    int num_cols = 0;
    
    char cols_copy[256];
    strncpy(cols_copy, columns, 255);
    cols_copy[255] = '\0';
    
    char* token = strtok(cols_copy, ",");
    while (token != NULL && num_cols < MAX_COLUMNS) {
        col_indices[num_cols] = atoi(token) - 1; // 转换为0基索引
        num_cols++;
        token = strtok(NULL, ",");
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        char* fields[MAX_COLUMNS];
        int field_count = 0;
        
        char* field = strtok(line, &delim_char);
        while (field != NULL && field_count < MAX_COLUMNS) {
            fields[field_count] = field;
            field_count++;
            field = strtok(NULL, &delim_char);
        }
        
        // 输出指定列
        for (int i = 0; i < num_cols; i++) {
            if (i > 0) printf(",");
            if (col_indices[i] < field_count) {
                printf("%s", fields[col_indices[i]]);
            }
        }
        printf("\n");
    }

    fclose(file);
}

void extract_columns_by_name(const char* filename, const char* columns) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    char delim_char;
    detect_delimiter(filename, &delim_char);

    char line[MAX_LINE_LENGTH];
    int col_indices[MAX_COLUMNS];
    int num_target_cols = 0;
    int found_indices[MAX_COLUMNS];
    
    // 解析目标列名
    char target_cols[MAX_COLUMNS][256];
    char cols_copy[256];
    strncpy(cols_copy, columns, 255);
    cols_copy[255] = '\0';
    
    char* token = strtok(cols_copy, ",");
    while (token != NULL && num_target_cols < MAX_COLUMNS) {
        trim_whitespace(token);
        strncpy(target_cols[num_target_cols], token, 255);
        target_cols[num_target_cols][255] = '\0';
        // 转换为小写以便比较
        for (int i = 0; target_cols[num_target_cols][i]; i++) {
            target_cols[num_target_cols][i] = tolower(target_cols[num_target_cols][i]);
        }
        found_indices[num_target_cols] = -1;
        num_target_cols++;
        token = strtok(NULL, ",");
    }

    // 读取表头并找到匹配的列
    if (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        char* field = strtok(line, &delim_char);
        int field_index = 0;
        
        while (field != NULL) {
            char field_lower[256];
            strncpy(field_lower, field, 255);
            field_lower[255] = '\0';
            
            // 转换为小写
            for (int i = 0; field_lower[i]; i++) {
                field_lower[i] = tolower(field_lower[i]);
            }
            
            // 检查是否匹配任何目标列名
            for (int i = 0; i < num_target_cols; i++) {
                if (strstr(field_lower, target_cols[i]) != NULL) {
                    found_indices[i] = field_index;
                }
            }
            
            field = strtok(NULL, &delim_char);
            field_index++;
        }
        
        // 输出表头
        for (int i = 0; i < num_target_cols; i++) {
            if (i > 0) printf(",");
            if (found_indices[i] >= 0) {
                // 重新读取表头以获取原始列名
                fseek(file, 0, SEEK_SET);
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\r\n")] = 0;
                
                char* header_field = strtok(line, &delim_char);
                int idx = 0;
                while (header_field != NULL && idx <= found_indices[i]) {
                    if (idx == found_indices[i]) {
                        printf("%s", header_field);
                        break;
                    }
                    header_field = strtok(NULL, &delim_char);
                    idx++;
                }
            }
        }
        printf("\n");
    }

    // 处理数据行
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        char* fields[MAX_COLUMNS];
        int field_count = 0;
        
        char* field = strtok(line, &delim_char);
        while (field != NULL && field_count < MAX_COLUMNS) {
            fields[field_count] = field;
            field_count++;
            field = strtok(NULL, &delim_char);
        }
        
        // 输出匹配的列
        for (int i = 0; i < num_target_cols; i++) {
            if (i > 0) printf(",");
            if (found_indices[i] >= 0 && found_indices[i] < field_count) {
                printf("%s", fields[found_indices[i]]);
            }
        }
        printf("\n");
    }

    fclose(file);
}

void convert_to_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    char delim_char;
    detect_delimiter(filename, &delim_char);

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        // 替换分隔符为逗号
        for (int i = 0; line[i]; i++) {
            if (line[i] == delim_char) {
                line[i] = ',';
            }
        }
        
        printf("%s\n", line);
    }

    fclose(file);
}

void check_file_consistency(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    char delim_char;
    delimiter_type_t delim_type = detect_delimiter(filename, &delim_char);

    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int expected_columns = 0;
    int inconsistent_lines = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        line[strcspn(line, "\r\n")] = 0;
        
        int column_count = 1; // 至少有一列
        if (delim_type == DELIM_MULTISPACE) {
            // 处理多空格分隔符
            char* ptr = line;
            int in_field = 0;
            column_count = 0;
            
            while (*ptr) {
                if (*ptr == ' ') {
                    if (in_field) {
                        in_field = 0;
                    }
                } else {
                    if (!in_field) {
                        column_count++;
                        in_field = 1;
                    }
                }
                ptr++;
            }
        } else {
            column_count = count_char_occurrences(line, delim_char) + 1;
        }
        
        if (line_number == 1) {
            expected_columns = column_count;
        } else if (column_count != expected_columns) {
            printf("不一致行号:%d, 列数:%d, 内容: %s\n", line_number, column_count, line);
            inconsistent_lines++;
        }
    }

    if (inconsistent_lines == 0) {
        printf("所有行列数相同 (分隔符: ");
        switch (delim_type) {
            case DELIM_TAB: printf("TAB"); break;
            case DELIM_COMMA: printf(","); break;
            case DELIM_SEMICOLON: printf(";"); break;
            case DELIM_PIPE: printf("|"); break;
            case DELIM_SPACE: printf("空格"); break;
            case DELIM_MULTISPACE: printf("多空格"); break;
            default: printf("未知"); break;
        }
        printf(")\n");
    } else {
        printf("共有 %d 行列数不同\n", inconsistent_lines);
    }

    fclose(file);
}

void show_column_headers(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    char delim_char;
    detect_delimiter(filename, &delim_char);

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        printf("列名和对应的列号:\n");
        
        char* token = strtok(line, &delim_char);
        int column_number = 1;
        
        while (token != NULL) {
            printf("%d: %s\n", column_number, token);
            token = strtok(NULL, &delim_char);
            column_number++;
        }
    }

    fclose(file);
}

void remove_duplicates(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    char lines[10000][MAX_LINE_LENGTH];
    int line_count = 0;
    char line[MAX_LINE_LENGTH];

    // 读取所有行
    while (fgets(line, sizeof(line), file) && line_count < 10000) {
        line[strcspn(line, "\r\n")] = 0;
        strcpy(lines[line_count], line);
        line_count++;
    }
    fclose(file);

    // 输出第一行（表头）
    if (line_count > 0) {
        printf("%s\n", lines[0]);
    }

    // 去重输出
    for (int i = 1; i < line_count; i++) {
        int is_duplicate = 0;
        
        // 检查是否与之前的行重复
        for (int j = 1; j < i; j++) {
            if (strcmp(lines[i], lines[j]) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            printf("%s\n", lines[i]);
        }
    }
}

void show_duplicates(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    char delim_char;
    delimiter_type_t delim_type = detect_delimiter(filename, &delim_char);

    printf("=== 重复行检测结果 ===\n");
    printf("分隔符: ");
    switch (delim_type) {
        case DELIM_TAB: printf("TAB\n"); break;
        case DELIM_COMMA: printf(",\n"); break;
        case DELIM_SEMICOLON: printf(";\n"); break;
        case DELIM_PIPE: printf("|\n"); break;
        case DELIM_SPACE: printf("空格\n"); break;
        case DELIM_MULTISPACE: printf("多空格\n"); break;
        default: printf("未知\n"); break;
    }
    printf("\n");

    char lines[10000][MAX_LINE_LENGTH];
    int line_numbers[10000];
    int line_count = 0;
    char line[MAX_LINE_LENGTH];
    int current_line = 0;

    // 读取所有行
    while (fgets(line, sizeof(line), file) && line_count < 10000) {
        current_line++;
        line[strcspn(line, "\r\n")] = 0;
        
        if (current_line == 1) {
            printf("表头: %s\n\n", line);
            continue;
        }
        
        strcpy(lines[line_count], line);
        line_numbers[line_count] = current_line;
        line_count++;
    }
    fclose(file);

    // 查找重复行
    int duplicate_groups = 0;
    int total_duplicates = 0;
    int processed[10000] = {0};

    for (int i = 0; i < line_count; i++) {
        if (processed[i]) continue;
        
        int group_size = 1;
        int group_lines[1000];
        group_lines[0] = line_numbers[i];
        
        for (int j = i + 1; j < line_count; j++) {
            if (!processed[j] && strcmp(lines[i], lines[j]) == 0) {
                group_lines[group_size] = line_numbers[j];
                group_size++;
                processed[j] = 1;
            }
        }
        
        if (group_size > 1) {
            duplicate_groups++;
            total_duplicates += group_size;
            
            printf("重复组 %d (出现 %d 次):\n", duplicate_groups, group_size);
            printf("行号: ");
            for (int k = 0; k < group_size; k++) {
                if (k > 0) printf(",");
                printf("%d", group_lines[k]);
            }
            printf("\n内容: %s\n\n", lines[i]);
        }
        
        processed[i] = 1;
    }

    if (duplicate_groups == 0) {
        printf("没有发现重复行\n");
    } else {
        printf("总结: 共有 %d 个重复组，涉及 %d 行数据\n", duplicate_groups, total_duplicates);
        int unique_lines = line_count - (total_duplicates - duplicate_groups);
        printf("唯一行数: %d\n", unique_lines);
        printf("重复率: %.2f%%\n", (float)(total_duplicates - duplicate_groups) * 100.0 / line_count);
    }
}

void split_string(const char* input, const char* delimiter) {
    char input_copy[MAX_LINE_LENGTH];
    strncpy(input_copy, input, MAX_LINE_LENGTH - 1);
    input_copy[MAX_LINE_LENGTH - 1] = '\0';

    char* delim_pattern = "、,;| ";
    if (delimiter && strlen(delimiter) > 0) {
        delim_pattern = (char*)delimiter;
    }

    char* token = strtok(input_copy, delim_pattern);
    while (token != NULL) {
        trim_whitespace(token);
        if (strlen(token) > 0) {
            printf("%s\n", token);
        }
        token = strtok(NULL, delim_pattern);
    }
}

void split_file_content(const char* filename, const char* delimiter) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    char* delim_pattern = "、,;| ";
    if (delimiter && strlen(delimiter) > 0) {
        delim_pattern = (char*)delimiter;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        split_string(line, delim_pattern);
    }

    fclose(file);
}

void process_fasta_list(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    printf("=== FASTA文件序列列表 ===\n");
    
    char line[MAX_LINE_LENGTH];
    int seq_count = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        if (line[0] == '>') {
            seq_count++;
            printf("%d: %s\n", seq_count, line + 1); // 跳过'>'符号
        }
    }
    
    printf("\n总序列数: %d\n", seq_count);
    fclose(file);
}

void process_fasta_extract(const char* filename, const char* sequence_names, const char* output_file) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return;
    }

    FILE* output = stdout;
    if (output_file) {
        output = fopen(output_file, "w");
        if (!output) {
            fprintf(stderr, "无法创建输出文件: %s\n", output_file);
            fclose(file);
            return;
        }
    }

    // 解析序列名列表
    char target_names[100][256];
    int num_targets = 0;
    
    char names_copy[1024];
    strncpy(names_copy, sequence_names, 1023);
    names_copy[1023] = '\0';
    
    char* token = strtok(names_copy, ",");
    while (token != NULL && num_targets < 100) {
        trim_whitespace(token);
        // 转换为小写以便比较
        for (int i = 0; token[i]; i++) {
            target_names[num_targets][i] = tolower(token[i]);
        }
        target_names[num_targets][strlen(token)] = '\0';
        num_targets++;
        token = strtok(NULL, ",");
    }

    char line[MAX_LINE_LENGTH];
    int in_target_sequence = 0;
    int found_any = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        if (line[0] == '>') {
            // 检查是否匹配目标序列
            char header_lower[512];
            strncpy(header_lower, line + 1, 511);
            header_lower[511] = '\0';
            
            // 转换为小写
            for (int i = 0; header_lower[i]; i++) {
                header_lower[i] = tolower(header_lower[i]);
            }
            
            in_target_sequence = 0;
            for (int i = 0; i < num_targets; i++) {
                if (strstr(header_lower, target_names[i]) != NULL) {
                    in_target_sequence = 1;
                    found_any = 1;
                    break;
                }
            }
            
            if (in_target_sequence) {
                fprintf(output, "%s\n", line);
            }
        } else if (in_target_sequence) {
            fprintf(output, "%s\n", line);
        }
    }

    if (!found_any) {
        fprintf(stderr, "未找到匹配的序列: %s\n", sequence_names);
        fprintf(stderr, "提示: 使用 '%s %s fasta list' 查看所有可用序列\n", "detect_delim", filename);
    } else if (output_file) {
        printf("序列已保存到: %s\n", output_file);
    }

    fclose(file);
    if (output != stdout) {
        fclose(output);
    }
}

int is_fasta_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file)) {
        fclose(file);
        return (line[0] == '>');
    }
    
    fclose(file);
    return 0;
}

data_type_t detect_data_type(const char* value) {
    if (strlen(value) == 0) {
        return DATA_EMPTY;
    }

    char* endptr;
    
    // 尝试解析为整数
    long int_val = strtol(value, &endptr, 10);
    if (*endptr == '\0') {
        return DATA_INTEGER;
    }
    
    // 尝试解析为浮点数
    double float_val = strtod(value, &endptr);
    if (*endptr == '\0') {
        return DATA_FLOAT;
    }
    
    return DATA_TEXT;
}

void trim_whitespace(char* str) {
    // 移除前导空白
    char* start = str;
    while (isspace(*start)) {
        start++;
    }
    
    // 移除尾随空白
    char* end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) {
        end--;
    }
    
    // 复制结果
    int len = end - start + 1;
    memmove(str, start, len);
    str[len] = '\0';
}

int count_char_occurrences(const char* str, char ch) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == ch) {
            count++;
        }
    }
    return count;
}

void format_file_size(long size, char* buffer) {
    if (size >= 1073741824) {
        sprintf(buffer, "%.2fGB", (double)size / 1073741824);
    } else if (size >= 1048576) {
        sprintf(buffer, "%.2fMB", (double)size / 1048576);
    } else if (size >= 1024) {
        sprintf(buffer, "%.2fKB", (double)size / 1024);
    } else {
        sprintf(buffer, "%ldB", size);
    }
}