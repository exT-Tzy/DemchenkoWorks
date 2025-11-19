--task 1

select p.participant_id, p.first_name, p.last_name, p.date_of_birth, p.gender
from public.participant p
where p.gender = 'male'
and p.date_of_birth <= '2007-11-03'::date
and exists (
    select 1 
    from public.solution s
    join public.task t on s.task_id = t.task_id
    join public.contest_info ci on t.contest_id = ci.contest_id
    where ci.contest_date = '2025-11-03'::date
    and s.participant_id = p.participant_id
);

--task 2

select task_id, task_name, task_description
from public.task
where task_description like '%переменная%'
and task_description like '%значение%';

--task 3

select p.first_name, p.last_name, date_part('year', age(current_date, p.date_of_birth)) as age
from public.participant p
where (
    select count(distinct t.contest_id)
    from public.solution s
    join public.task t on s.task_id = t.task_id
    where s.participant_id = p.participant_id
) >= 3;

--task 4

select distinct t.task_id, t.task_name, t.task_description
from public.task t
join public.solution s on t.task_id = s.task_id
where s.grade = 100;

--task 5

select t.task_id, t.task_name, t.task_description
from public.task t
where not exists (
    select 1
    from public.solution s
    where s.task_id = t.task_id
    and s.grade >= 52
    and s.programming_language_name = 'Py***n'
);

--task 6

select distinct contest_name
from public.contest_info
where extract(month from contest_date) in (6, 7, 8);

--task 7

select programming_language_name, programming_language_version
from public.solution
where grade >= 85
group by programming_language_name, programming_language_version
having count(task_id) >= 2;

--task 8

select distinct t.task_id, t.task_name, t.task_description
from public.task t
join public.solution s on t.task_id = s.task_id
where s.programming_language_name = 'C'
and s.programming_language_version = 'C99'
and s.text_of_solution like '%#include <string.h>%';

--task 9

select ci.contest_id, ci.contest_name, s.programming_language_name, s.programming_language_version
from public.contest_info ci
join public.task t on ci.contest_id = t.contest_id
join public.solution s on t.task_id = s.task_id
group by ci.contest_id, ci.contest_name, s.participant_id, s.programming_language_name, s.programming_language_version
having count(distinct t.task_id) = (
    select count(distinct task_id) 
    from public.task 
    where contest_id = ci.contest_id
)
and avg(s.grade) > 75
and min(s.grade) >= 50
)
group by ci.contest_id, ci.contest_name, s.programming_language_name, s.programming_language_version
having count(distinct s.participant_id) >= 3;

--task 10

select distinct s.programming_language_name
from public.solution s
join public.task t on s.task_id = t.task_id
join public.contest_info ci on t.contest_id = ci.contest_id
where ci.contest_name = 'поставьте 3 плиз';

--task 11

select p.participant_id, p.first_name, p.last_name
from public.participant p
where exists (
    select 1
    from public.solution s
    join public.task t on s.task_id = t.task_id
    join public.contest_info ci on t.contest_id = ci.contest_id
    where ci.contest_name = 'памагити((9(9'
    and s.participant_id = p.participant_id
    group by s.participant_id
    having count(distinct t.task_id) = (
        select count(distinct task_id)
        from public.task
        where contest_id = ci.contest_id
    )
    and min(s.grade) >= 45
    and count(distinct s.programming_language_name || s.programming_language_version) = count(distinct t.task_id)
);

--task 12

select distinct p.participant_id, p.first_name, p.last_name
from public.participant p
where p.gender = 'female'
and exists (
    select 1
    from public.solution s
    join public.task t on s.task_id = t.task_id
    where s.participant_id = p.participant_id
    and s.grade >= 52
    group by t.contest_id
    having count(distinct s.task_id) >= 3
);

--task 13

select distinct p.participant_id, p.first_name, p.last_name
from public.participant p
where p.last_name = 'Демченко'
and exists (
    select 1
    from public.solution s
    join public.task t on s.task_id = t.task_id
    join public.contest_info ci on t.contest_id = ci.contest_id
    where ci.contest_name = 'no way back'
    and ci.contest_date = '2025-11-03'::date
    and s.participant_id = p.participant_id
    and s.grade = 0
    and s.programming_language_name = 'C++'
    and s.text_of_solution like '%unsigned double%'
    group by s.participant_id
    having count(distinct t.task_id) = (
        select count(distinct task_id)
        from public.task
        where contest_id = ci.contest_id
    )
);

--task 14

select t.task_id, t.task_name, avg(s.grade) as average_grade
from public.task t
join public.solution s on t.task_id = s.task_id
where s.grade >= 70
group by t.task_id, t.task_name
having avg(s.grade) between 80 and 85;

--task 15

select distinct t.task_id, t.task_name, t.task_description
from public.task t
join public.solution s on t.task_id = s.task_id
where s.reviewer_comment like '%где лабы?%';