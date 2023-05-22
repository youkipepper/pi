import pymysql
from datetime import datetime, timedelta
import time

# 生成表格连接信息
table_db_info = {
    'host': '123.60.191.124',
    'user': 'root',
    'password': 'Hzk2022@',
    'database': 'hhu_int_lab'
}

# 生成表格的函数
def generate_tables():
    # 获取当前日期和时间
    now = datetime.now()

    # 连接生成表格的数据库
    table_conn = pymysql.connect(**table_db_info)
    if table_conn.open:
        print("Connected to Table DB!")

    try:
        with table_conn.cursor() as cursor:
            for i in range(7):
                # 计算当前表格的日期，并生成表格名字
                table_date = now + timedelta(days=i)
                table_name = 'table_{}'.format(table_date.strftime('%Y%m%d'))

                # 使用固定结构创建新表格
                create_sql = f"""
                                CREATE TABLE {table_name} (
                                    `id` INT(16) NOT NULL AUTO_INCREMENT COMMENT '序号，自增',
                                    `x_offset` DOUBLE(255,10) NOT NULL DEFAULT '0.0000000000' COMMENT '横向位移',
                                    `y_offset` DOUBLE(255,10) NOT NULL DEFAULT '0.0000000000' COMMENT '纵向位移',
                                    `frame_num` INT(11) NOT NULL DEFAULT '0' COMMENT '当前数据帧号',
                                    `test_point_code` VARCHAR(255) NOT NULL DEFAULT '1' COMMENT '对应的测点号',
                                    `bucket_name` VARCHAR(1024) NOT NULL DEFAULT '1' COMMENT '桶名',
                                    `obj_key` VARCHAR(2048) NOT NULL COMMENT '文件在桶中的键',
                                    `video_record_time` DATETIME(6) NOT NULL COMMENT '当前时间',
                                    PRIMARY KEY (`id`),
                                    KEY `ind_ana_time` (`video_record_time`),
                                    KEY `ind_ana_code` (`test_point_code`)
                                ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8mb4 COMMENT='位移数据记录';
                            """
                cursor.execute(create_sql)

        print('Tables generated successfully!')

    finally:
        # 关闭生成表格的数据库连接
        table_conn.close()

# 运行程序
while True:
    now = datetime.now()
    if now.weekday() != 0:
        time.sleep(3600) # 睡眠1小时
        continue
    generate_tables()
    time.sleep(86400) # 每日生成一次表格，睡眠24小时
