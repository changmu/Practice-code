# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://docs.scrapy.org/en/latest/topics/item-pipeline.html


# useful for handling different item types with a single interface
import pymysql
import scrapy.crawler
from itemadapter import ItemAdapter


class TaobaoPipeline:
    @classmethod
    def from_crawler(cls, crawler: scrapy.crawler.Crawler):
        mysql_conf = crawler.settings["MysqlConf"]
        user = mysql_conf["user"]
        password = mysql_conf["password"]
        host = mysql_conf["host"]
        port = mysql_conf["port"]
        db_name = mysql_conf["db_name"]
        return cls(user, password, host, port, db_name)

    def __init__(self, user, password, host, port, db_name):
        self.conn = pymysql.connect(
            user=user,
            password=password,
            host=host,
            port=port,
            database=db_name,
            autocommit=True,
            charset="utf8mb4",
        )
        self.cursor = self.conn.cursor()

    def __del__(self):
        self.conn.close()

    def process_item(self, item, spider):
        # TODO
        return item
