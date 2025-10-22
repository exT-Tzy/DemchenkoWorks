-- task 1

select first_name, middle_name, last_name, course_number
	from public.student s
	inner join public.student_group sg 
		on s.group_id = sg.group_id
	where sg.course_number = 3;
	

-- task 2

select group_code, count(student_id) as students_count
	from public.student_group sg 
	inner join public.student s 
		on sg.group_id = s.group_id	
	group by group_code, course_number
	having count(student_id) > 9
	order by students_count desc;


-- task 3

select subject_name, semester_number, avg(grade) as average_grade
	from public.subject s 
	inner join public.student_mark sm 
		on s.subject_id = sm.subject_id 
	where s.assessment_type = 'exam'
		and sm.semester_number = 4
	group by subject_name, s.assessment_type, semester_number
	having avg(grade) > 3.5;
	

-- task 4

select first_name, middle_name, last_name
	from public.student s 
	inner join student_mark sm 
		on s.student_id = sm.student_id
	group by first_name, middle_name, last_name, semester_number
	having sm.semester_number = 3
		and avg(grade) = 5
	order by middle_name
	limit 10;


-- task 5

select subject_name
	from public.subject s 
	inner join public.student_mark sm 
		on s.subject_id = sm.subject_id
	where s.assessment_type = 'exam'
		and sm.semester_number between 5 and 8 
	group by s.subject_name
	order by subject_name;


-- task 6

select semester_number
from(
	select sm.semester_number, s.student_id 
	from public.student_mark sm 
	inner join public.student s 
		on sm.student_id = s.student_id
	inner join public.student_group sg 
		on s.group_id = sg.group_id 
	inner join public.subject s2 
		on sm.subject_id = s2.subject_id 
	where sg.group_code = 'ИТПМ-124'
		and s2.assessment_type = 'exam'
	group by semester_number, s.student_id 
	having avg(grade) > 4.5)
group by semester_number 
having count(distinct student_id) >= 3
order by semester_number;


--task 7
	
select first_name, middle_name, last_name, group_code, course_number, date_of_birth
	from student s 
	inner join public.student_group sg 
		on s.group_id = sg.group_id
	where s.gender = 'M'
		and sg.course_number in (2, 4)
	order by date_of_birth asc
	limit 5;


-- task 8

select subject_name, avg(grade) as average_grade
	from subject s 
	inner join public.student_mark sm 
		on s.subject_id = sm.subject_id 
	inner join public.student s2 
		on sm.student_id = s2.student_id
	inner join public.student_group sg 
		on s2.group_id = sg.group_id
	where sg.group_code = 'ИТПМ-124'
		and s.assessment_type = 'exam'
	group by subject_name
	having avg(grade) between 3.0 and 4.0
	order by s.subject_name;


-- task 9

select middle_name
	from public.student s 
	inner join public.student_mark sm 
		on s.student_id = sm.student_id
	inner join public.subject s2 
		on sm.subject_id = s2.subject_id
	where s2.subject_name = 'Database Systems'
		and sm.grade = 2
		and sm.semester_number = 3
	order by middle_name;


-- task 10

select first_name, middle_name, last_name, group_code
	from public.student s 
	inner join public.student_group sg 
		on s.group_id = sg.group_id
	inner join public.student_mark sm 
		on s.student_id = sm.student_id
	inner join public.subject s2 
		on sm.subject_id = s2.subject_id
	where s2.subject_name = 'Information Protection'
		and sm.semester_number = 4
		and sm.grade != 2
	order by middle_name;
	