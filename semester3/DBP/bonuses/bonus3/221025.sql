CREATE DATABASE bonus;

CREATE TABLE IF NOT EXISTS public.service
(
    id SERIAL NOT NULL PRIMARY KEY,
    name VARCHAR(150) NOT NULL
);

CREATE TABLE IF NOT EXISTS public.subscription
(
    id SERIAL NOT NULL PRIMARY KEY,
    name VARCHAR(150) NOT NULL,
    price MONEY NOT NULL,
    description TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS public.client
(
    id SERIAL NOT NULL PRIMARY KEY,
    surname VARCHAR(150) NOT NULL,
    name VARCHAR(150) NOT NULL,
    sex BOOL NOT NULL,
    birthday DATE NOT NULL,
    subscription_id INT NOT NULL,
    subscription_start DATE NOT NULL,
    subscription_end DATE NOT NULL,
    FOREIGN KEY(subscription_id) REFERENCES public.subscription(id)
);

CREATE TABLE IF NOT EXISTS public.sub_to_service
(
    sub_id INT NOT NULL,
    service_id INT NOT NULL,
    PRIMARY KEY (sub_id, service_id),
    FOREIGN KEY(sub_id) REFERENCES public.subscription(id),
    FOREIGN KEY(service_id) REFERENCES public.service(id)
);

--task 1
select c.surname
from public.client c 
inner join public.subscription s
	on c.subscription_id = s.id
where s.name = "demo_sub";

--task2
select c.name, c.surname
from public.client c 
where year(c.subscription_end) = year(curdate());

--task3
select s.name, count(c.id) as clients_count
from public.subscription s
inner join client c 
	on public."subscription".id = c.subscription_id
group by s.name, clients_count
having count(c.id) > 9;

--task4
select s.name, count(sts.service_id) as services_count
from public.subscription s
inner join public.sub_to_service sts 
	on s.id = sts.sub_id
group by s.name, services_count 
having count(sts.service_id) > 5;

--task 5
select c.name, c.surname
from public.client c
inner join subscription s on c.subscription_id = s.id
inner join sub_to_service sts on s.id = sts.sub_id
inner join service serv on sts.service_id = serv.id
where c.sex = true
    and  serv.name = 'отчислиться на АиСД';


--task 6
select sub.name as subscription_name, s.name as service_name
from subscription sub
inner join  sub_to_service sub_t on sub.id = sub_t.sub_id
inner join service s on s.id = sub_t.service_id;

--task 7
select c.name, c.surname
from public.client c 
where curdate() > c.subscription_end;

--task 8
select c.name, c.surname
    from public.client c
    inner join public.subscription s
        on c.subscription_id = s.id
    where s.name = 'ПМИ'
        and c.subscription_end = '03.11.2025'::DATE;

--task 9
select c.id, c.surname, c.name--[, ...
from client c
join subscription s on s.id = c.subscription_id
where s.price > 52000;

--task 10
select c.name, c.surname
from public.client c
inner join public.subscription s
	on c.subscription_id = s.id
inner join public.sub_to_service sts 
	on s.id = sts.sub_id
where c.sex = false
	and sts.service_id = 3;

--task 11
select sv.name
from public.service sv
inner join public.sub_to_service sts 
	on sv.id = sts.service_id
inner join public.subscription sb
	on sts.sub_id = sb.id
inner join public.client c
	on sb.id = c.subscription_id
where curdate() < c.subscription_end;

--task 12
update public.client c 
set subscription_id = 2,
	subscription_start = curdate(),
	subscription_end = curdate() + interval '1 month'
where id = 1;

--task 13
update public.client c
set subscription_id = (
    select id 
    from public.subscription 
    where name = 'aboba'
), 
subscription_start = curdate(),
subscription_end = cur_date() + INTERVAL '1 month'
WHERE id = 1;

--task 14
SELECT COUNT(*)
  from public.client c
    inner join public.subscription su on c.subscription_id = su.id
    inner join public.sub_to_service sts on su.id = sts.sub_id
    inner join public.service se on sts.service_id = se.id
  where extract(year from (NOW() - c.birthday)) > 35
    and su.name = 'Бонусы для 2 курса'
    and c.sex = true;

--task 15
select 
    s.id,
    s.name,
    s.price,
    s.description,
    (select count(*) from public.sub_to_service where sub_id = s.id) as service_count,
    (select count(distinct id) from public.client where subscription_id = s.id and sex = true) as male_clients,
    (select count(distinct id) from public.client where subscription_id = s.id and sex = false) as female_clients
from public.subscription s
where s.price::numeric > 85000
    and (select count(*) from public.sub_to_service where sub_id = s.id) <= 10
    and (select count(distinct id) from public.client where subscription_id = s.id and sex = true) >= 10
    and (select count(distinct id) from public.client where subscription_id = s.id and sex = false) >= 10;