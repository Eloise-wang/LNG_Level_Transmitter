import csv
import re
import os
import glob

# 自动寻找当前目录下的第一个 .csv 文件
def find_csv_file():
    csv_files = glob.glob("*.csv")
    if not csv_files:
        return None
    return csv_files[0] 

OUTPUT_C_FILE = 'LNG_Arrays_Generated.c'

# 列索引 0对应1000L，1对应1350L，2对应1500L
column_configs = [
    {"col_index": 0, "array_name": "RawTable_1000L_850mm", "max_pf": 307},
    {"col_index": 1, "array_name": "RawTable_1350L_1000mm", "max_pf": 365},
    {"col_index": 2, "array_name": "RawTable_1500L_1050mm", "max_pf": 385}
]

pattern = re.compile(r'(\d+)pF\(\+\d+pF,(\d+)L,(\d+)mm\)')

def generate_c_arrays():
    input_csv = find_csv_file()
    
    if not input_csv:
        print("❌ 错误: 在当前文件夹下找不到任何 .csv 文件！")
        return
        
    print(f"🔍 找到文件: '{input_csv}'，正在尝试解析...")

    encodings_to_try = ['utf-8-sig', 'gbk', 'gb18030', 'utf-8']
    reader = None
    
    for enc in encodings_to_try:
        try:
            with open(input_csv, mode='r', encoding=enc) as f:
                reader = list(csv.reader(f))
            print(f"✅ 成功读取文件 (使用的编码: {enc})")
            break  
        except UnicodeDecodeError:
            continue  
            
    if reader is None:
        print("❌ 错误: 尝试了所有常见编码，仍无法读取。")
        return

    output_lines = []
    output_lines.append("/* 自动生成的 LNG 查表数据 (高密度排版) */\n")
    output_lines.append("#include \"LNG_Level_Table.h\"\n\n")

    # 设定每行排放的数组元素个数
    ITEMS_PER_LINE = 8 

    for config in column_configs:
        col_idx = config["col_index"]
        arr_name = config["array_name"]
        max_pf = config["max_pf"]
        
        output_lines.append(f"static const LngLevelData_t {arr_name}[{max_pf + 1}] = {{")
        
        # 1. 提取当前列的所有有效数据
        data_points = []
        for row_idx in range(5, len(reader)):
            if col_idx >= len(reader[row_idx]):
                continue
            cell_str = reader[row_idx][col_idx]
            if not cell_str.strip():
                continue
                
            match = pattern.search(cell_str)
            if match:
                pf_val = int(match.group(1))
                vol_val = int(match.group(2))
                height_val = int(match.group(3))
                data_points.append((pf_val, vol_val, height_val))
                
                if pf_val >= max_pf:
                    break
        
        # 2. 按块(分行)格式化排版
        for i in range(0, len(data_points), ITEMS_PER_LINE):
            chunk = data_points[i : i + ITEMS_PER_LINE]
            
            # 拼接结构体字符串，例如: {   0,  23}, {   0,  25}
            chunk_strs = [f"{{{v:>4}, {h:>4}}}" for p, v, h in chunk]
            line_content = ", ".join(chunk_strs) + ","
            
            # 提取当前行的电容起始和结束范围用于注释
            start_pf = chunk[0][0]
            end_pf = chunk[-1][0]
            
            # 左对齐 115 个字符宽度，保证右侧注释完美对齐
            formatted_line = f"    {line_content:<115} // {start_pf:>3} ~ {end_pf:>3} pF"
            output_lines.append(formatted_line)
                
        output_lines.append("};\n")

    with open(OUTPUT_C_FILE, mode='w', encoding='utf-8') as f:
        f.write("\n".join(output_lines))
        
    print(f"🎉 成功! 紧凑版代码已生成到 {OUTPUT_C_FILE}")

if __name__ == "__main__":
    generate_c_arrays()