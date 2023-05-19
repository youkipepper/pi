import mysql.connector as mysql
import pandas as pd
from datetime import datetime, timedelta

# 模板表连接信息
template_db_info = {
    'host': '123.60.191.124',
    'user': 'root',
    'password': 'Hzk2022@',
    'database': 'bridge'
}

# 生成表格连接信息
table_db_info = {
    'host': '123.60.191.124',
    'user': 'root',
    'password': 'Hzk2022@',
    'database': 'hhu_int_lab'
}

# 模板表名
template_table_name = 'hzk_analyzed_data'

# 生成表格的函数


def generate_tables():
    # 获取当前日期和时间
    now = datetime.now()

    # 如果今天不是星期一或者现在不到早上8点，则退出函数
    # if now.weekday() != 4 or now.hour != 21 or now.minute != 17:
    if now.weekday() != 4:
        print("It is not correct time. Current time is:", now)
        return

    # 连接模板表数据库
    template_conn = mysql.connect(**template_db_info)
    if template_conn.is_connected():
        print("Connected to Template Table DB!")

    try:
        with template_conn.cursor() as cursor:
            # 查询模板表格的结构
            sql = 'SHOW CREATE TABLE {}'.format(template_table_name)
            cursor.execute(sql)
            table_structure = cursor.fetchone()[1]
            print(table_structure)

    finally:
        # 关闭模板表数据库连接
        template_conn.close()

    # 修改模板表格结构，去掉 AUTO_INCREMENT 和 COMMENT 部分
    table_structure = table_structure.replace('AUTO_INCREMENT=58467527', '')
    table_structure = table_structure.replace('COMMENT', '')

    # 连接生成表格的数据库
    table_conn = mysql.connect(**table_db_info)
    if table_conn.is_connected():
        print("Connected to Table DB!")

    try:
        with table_conn.cursor() as cursor:
            for i in range(7):
                # 计算当前表格的日期，并生成表格名字
                table_date = now + timedelta(days=i)
                table_name = 'table_{}'.format(table_date.strftime('%Y%m%d'))

                # 使用修改后的模板表格结构创建新表格
                create_sql = 'CREATE TABLE {} ({})'.format(
                    table_name, table_structure)
                cursor.execute(create_sql)

        print('Tables generated successfully!')

    finally:
        # 关闭生成表格的数据库连接
        table_conn.close()


# 运行程序
while True:
    generate_tables()
