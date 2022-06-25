use test;
show tables;
show create table t_spider;

CREATE TABLE `t_taobao` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `ranking` int NOT NULL DEFAULT '0' COMMENT '排名',
  `title` varchar(128) not null DEFAULT '' COMMENT '书名',
  `rating` varchar(128) not null DEFAULT '' COMMENT '评分',
  `author` varchar(128) not null DEFAULT '' COMMENT '作者',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=278 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='豆瓣图书表'