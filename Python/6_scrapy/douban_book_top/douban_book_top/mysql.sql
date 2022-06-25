use test;
drop table t_spider;


create table t_spider (
    id bigint unsigned auto_increment comment '自增id',
    ranking int not null default '0' comment '排名',
    title varchar(128) default '' comment '书名',
    rating varchar(128) default '' comment '评分',
    author varchar(128) default '' comment '作者',
    primary key (id)
) engine=innodb comment='豆瓣图书表';

show tables;
show create table t_spider;

select * from t_spider;
select count(1) from t_spider;
delete from t_spider where id in(1,2);
