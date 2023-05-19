import pandas as pd
import pymysql
from datetime import timedelta, datetime

# 连接MySQL数据库
conn = pymysql.connect(
    host='localhost',  # MySQL服务器地址
    user='root',  # 用户名
    password='password',  # 密码
    database='test'  # 数据库名称
)

# 模板表格名称
template_table_name = 'hzk_analyzed_data_template'

# 定义函数，生成表格并插入到MySQL中
def generate_tables():
    # 获取当前时间所在周的日期范围
    today = datetime.now()
    start_of_week = today - timedelta(days=today.weekday())
    end_of_week = start_of_week + timedelta(days=6)

    # 生成7个新表格，并插入到MySQL中
    for i in range(7):
        date_str = (start_of_week + timedelta(days=i)).strftime('%Y%m%d')
        new_table_name = 'hzk_analyzed_data_' + date_str

        # 创建新表格
        sql_create_table = f'''
            CREATE TABLE {new_table_name} LIKE {template_table_name}
        '''
        with conn.cursor() as cursor:
            cursor.execute(sql_create_table)

        # 获取模板表格的数据，并修改日期列为当前日期
        df = pd.read_sql(f'SELECT * FROM {template_table_name}', con=conn)
        df['date'] = (start_of_week + timedelta(days=i)).strftime('%Y-%m-%d')

        # 将修改后的数据插入到新表格中
        df.to_sql(new_table_name, con=conn, if_exists='append', index=False)

    # 打印消息，表明已经生成了新的表格
    print('Tables generated successfully.')

# 定义触发器函数，每周五下午3点运行生成表格函数
def trigger_function():
    now = datetime.now()
    if now.weekday() == 4 and now.hour == 15 and now.minute == 0 and now.second == 0:
        generate_tables()

# 循环运行触发器函数
while True:
    trigger_function()
