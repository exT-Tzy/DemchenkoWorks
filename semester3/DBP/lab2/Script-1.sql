-- task 1

create or replace function add_author(
	first_name varchar,
	last_name varchar)
returns int as $$
declare
	generated_id int;
begin
	insert into public.author(author_first_name, 
							  author_last_name)
	values (first_name, 
			last_name)
	returning public.author.author_id into generated_id;
	return generated_id;
end;
$$ language plpgsql;


select add_author('Совунья', 'Смешарикова'); -- add example
select add_author('Копатыч', 'Смешарикович');
select add_author('Всем-Ку', 'Я-Уже-Смешариков');


create or replace function delete_author(
	param_id int)
returns void as $$
begin
	if exists (select 1 from public.book as pb
			   where pb.author_id = param_id)
	then 
		raise exception 'author cannot be deleted because there are books linked with him';
	end if;

	delete from public.author as pa
		where pa.author_id = param_id;
end;
$$ language plpgsql;

delete_author(3); -- delete example


create or replace function update_author(
	param_id int,
	new_first_name varchar,
	new_last_name varchar)
returns void as $$
begin
	update public.author
	set 
		author_first_name = new_first_name,
		author_last_name = new_last_name
	where author_id = param_id;
end;
$$ language plpgsql;

update_author(2, 'Копатыч', 'Смешариков'); -- update example


-- task 2


create or replace function add_publishing_house(
	name varchar,
	city varchar)
returns int as $$
declare
	generated_id int;
begin
	insert into public.publishing_house(publishing_house_name,
										publishing_house_city)
	values (name,
			city)
	returning public.publishing_house.publishing_house_id into generated_id;
	return generated_id;
end;
$$ language plpgsql;

--select add_publishing_house('kirieshki', 'samara');


create or replace function delete_publishing_house(
	param_id int)
returns void as $$
begin
	if exists (select 1 from public.book as pb
			   where pb.publishing_house_id = param_id)
	then
		raise exception 'publishing house cannot be deleted because there are books linked with it';
	end if;

	delete from public.publishing_house as ph
		where ph.publishing_house_id = param_id;
end;
$$ language plpgsql;


create or replace function update_publishing_house(
	param_id int,
	new_name varchar,
	new_city varchar)
returns void as $$
begin
	update public.publishing_house
	set 
		publishing_house_name = new_name,
		publishing_house_city = new_city
	where publishing_house_id = param_id;
end;
$$ language plpgsql;


-- task 3


create or replace function add_book(
	param_title varchar,
	param_author_id int,
	param_publishing_house_id int,
	param_publication int,
	param_publication_year int,
	param_circulation int)
returns int as $$
declare
	generated_id int;
begin
	if not exists (select 1 from public.author as pa
				   where pa.author_id = param_author_id)
	then 
		raise exception 'this author doesnt exist';
	end if;

	if not exists (select 1 from public.publishing_house as ph
				   where ph.publishing_house_id = param_publishing_house_id)
	then 
		raise exception 'this publishing house doesnt exist';
	end if;
	
	insert into public.book(book_title,
							author_id,
							publishing_house_id,
							book_publication,
							book_publication_year,
							book_circulation)
	values (param_title,
			param_author_id,
			param_publishing_house_id,
			param_publication,
			param_publication_year,
			param_circulation)
	returning public.book.book_id into generated_id;
	return generated_id;
end;
$$ language plpgsql;

--select add_book('mega wow nazvanie', 4, 2, 1, 2025, 1);

create or replace function delete_book(
	param_id int)
returns void as $$
begin
	if exists (select 1 from public.booking as pb
			   where pb.book_id = param_id)
	then
		raise exception 'book cannot be deleted because there are booked books boo booboboob bobobo boooook';
	end if;

	if exists (select 1 from public.book_instance as bi
			   where bi.book_id = param_id)
	then
		raise exception 'book cannot be deleted because there are linked lines in book_instance table';
	end if;
	
	delete from public.book
	where book_id = param_id;
end;
$$ language plpgsql;


create or replace function update_book(
	param_book_id int,
	new_title varchar,
	new_author_id int,
	new_publishing_house_id int,
	new_book_publication int,
	new_book_publication_year int,
	new_book_circulation int)
returns void as $$
begin
	if not exists (select 1 from public.author as pa
				   where pa.author_id = new_author_id)
	then 
		raise exception 'this author doesnt exist';
	end if;

	if not exists (select 1 from public.publishing_house as ph
				   where ph.publishing_house_id = new_publishing_house_id)
	then 
		raise exception 'this publishing house doesnt exist';
	end if;
	
	update public.book 
	set
		book_title = new_title,
		author_id = new_author_id,
		publishing_house_id = new_publishing_house_id,
		book_publication = new_book_publication,
		book_publication_year = new_book_publication_year,
		book_circulation = new_book_circulation
	where book_id = param_book_id;
end;
$$ language plpgsql;


-- task 4


create or replace function add_reader(
	reader_id int,
	first_name varchar,
	last_name varchar,
	date_of_birth date,
	param_reader_gender gender,
	date_of_registration date)
returns void as $$
begin
	insert into public.reader(reader_card_id,
							  reader_first_name,
							  reader_last_name,
							  reader_date_of_birth,
							  reader_gender,
							  reader_date_of_registration)
	values (reader_id,
			first_name,
			last_name,
			date_of_birth,
			param_reader_gender,
			date_of_registration);
end;
$$ language plpgsql;

select add_reader(2, 'Sasha', 'Kirieshkin', '01.01.2001', 'female', '01.01.1999');

create or replace function delete_reader(
	param_id int)
returns void as $$
begin
	if exists (select 1 from public.booking as pb
			   where pb.reader_id = param_id)
	then
		raise exception 'u cant delete him because of booking table';
	end if;
	
	if exists (select 1 from public.issuance as i
			   where i.reader_id = param_id)
	then
		raise exception 'u cant delete him because of issuance table';
	end if;
	
	delete from public.reader
	where reader_id = param_id;
end;
$$ language plpgsql;


create or replace function update_reader(
	param_id int,
	new_first_name varchar,
	new_last_name varchar,
	new_date_of_birth date,
	new_gender gender, -- LOL
	new_date_of_registration date)
returns void as $$
begin
	update public.reader
	set
		reader_first_name = new_first_name,
		reader_last_name = new_last_name,
		reader_date_of_birth = new_date_of_birth,
		reader_gender = new_gender, -- ?????????
		reader_date_of_registration = new_date_of_registration
	where reader_id = param_id;
end;
$$ language plpgsql;


-- task 5


create or replace function add_book_instance(
	param_book_inventory_id int,
	param_book_id int,
	param_state state,
	param_status status,
	param_location varchar)
returns void as $$
begin
	if not exists (select 1 from public.book as pb
				   where pb.book_id = param_book_id)
	then
		raise exception 'this book doesnt exist';
	end if;

	insert into public.book_instance(book_inventory_id,
									 book_id,
									 book_state,
									 book_status,
									 book_location)
	values (param_book_inventory_id,
			param_book_id,
			param_state,
			param_status,
			param_location);
end;
$$ language plpgsql;

select add_book_instance(6, 2, 'good', 'in stock', '/2/1/2');

create or replace function delete_book_instance(
	param_id int)
returns void as $$
begin
	if exists (select 1 from public.issuance as i
			   where i.book_inventory_id = param_id)
	then
		raise exception 'cannot be deleted because of issuance table';
	end if;

	delete from public.book_instance
	where book_inventory_id = param_id;
end;
$$ language plpgsql;


create or replace function update_book_instance(
	param_id int,
	new_book_id int,
	new_state state,
	new_status status,
	new_location varchar)
returns void as $$
begin
	if not exists (select 1 from public.book as pb
				   where pb.book_id = new_book_id)
	then 
		raise exception 'this book doesnt exist';
	end if;
	
	update public.book_instance 
	set
		book_id = new_book_id,
		book_state = new_state,
		book_status = new_status,
		book_location = new_location
	where book_inventory_id = param_id;
end;
$$ language plpgsql;


-- task 6
select issue_book(2, 2, '01.01.2001 00:00:01', '01.01.2012');

create or replace function issue_book(
	param_book_id int,
	param_reader_id int,
	param_issuance_datetime timestamp,
	param_expected_refund_date date)
returns void as $$
begin
	if not exists (select 1 from public.book as pb
				   where pb.book_id = param_book_id)
	then 
		raise exception 'this book doesnt exist';
	end if;

	if not exists (select 1 from public.reader as pr
				   where pr.reader_card_id = param_reader_id)
	then
		raise exception 'this reader doesnt exist';
	end if;

	if not exists (select 1 from public.book_instance
				   where book_inventory_id = param_book_id
				   and book_status = 'in stock')
	then
		raise exception 'this book cannot be issued';
	end if;
	
	insert into public.issuance(book_inventory_id,
								reader_card_id,
								issuance_datetime,
								expected_refund_date)
	values (param_book_id,
			param_reader_id,
			param_issuance_datetime,
			param_expected_refund_date);

	update public.book_instance 
    set book_status = 'issued' 
    where book_inventory_id = param_book_id;
end;
$$ language plpgsql;


-- task 7


create or replace function refund_book(
	param_book_id int,
	param_reader_id int,
	param_actual_refund_date date)
returns void as $$
begin
	update public.issuance
	set actual_refund_date = param_actual_refund_date
	where book_inventory_id = param_book_id and reader_card_id = param_book_id;

	update public.book_instance
	set book_status = 'in stock'
	where book_inventory_id = param_book_id;
end;
$$ language plpgsql;


-- task 8


create view issued_books as
select reader_first_name, 
	   reader_last_name, 
	   author_first_name, 
	   author_last_name, 
	   book_title, 
	   book_state, 
	   issuance_datetime
from public.issuance i
inner join public.reader r 
	on i.reader_card_id = r.reader_card_id
inner join public.book_instance bi 
	on i.book_inventory_id  = bi.book_id 
inner join public.book b 
	on bi.book_id = b.book_id 
inner join public.author a 
	on b.author_id = a.author_id 
where i.actual_refund_date is null
order by issuance_datetime desc;


-- task 9


create view overdue_books as
select reader_first_name,
	   reader_last_name,
	   author_first_name,
	   author_last_name,
	   book_title,
	   (CURRENT_DATE - expected_refund_date) as overdue_days
from public.issuance i
inner join public.reader r
	on i.reader_card_id = r.reader_card_id
inner join public.book_instance bi 
	on i.book_inventory_id = bi.book_inventory_id 
inner join public.book b 
	on bi.book_id = b.book_id 
inner join public.author a 
	on b.author_id = a.author_id 
where i.actual_refund_date is null
	and i.expected_refund_date < CURRENT_DATE
order by overdue_days desc;


-- task 10


create or replace function modified_issue_book(
	param_book_id int,
	param_reader_id int,
	param_issuance_datetime timestamp,
	param_expected_refund_date date)
returns void as $$
begin
	if not exists (select 1 from public.book as pb
				   where pb.book_id = param_book_id)
	then 
		raise exception 'this book doesnt exist';
	end if;

	if not exists (select 1 from public.reader as pr
				   where pr.reader_id = param_reader_id)
	then
		raise exception 'this reader doesnt exist';
	end if;

	if not exists (select 1 from public.book_instance
				   where book_inventory_id = param_book_id
				   and book_status = 'in stock')
	then
		raise exception 'this book cannot be issued';
	end if;

	if exists(
		select 1 from overdue_books
		where reader_card_id = param_reader_id)
	then
		raise exception 'gde kniga lebovski???';
	end if;
	
	insert into public.issuance(book_inventory_id,
								reader_card_id,
								issuance_datetime,
								expected_refund_date)
	values (param_book_id,
			param_reader_id,
			param_issuance_datetime,
			param_expected_refund_datetime);

	update public.book_instance 
    set book_status = 'issued' 
    where book_inventory_id = param_book_inventory_id;
end;
$$ language plpgsql;


-- task 11


create or replace function book_reservation(
	param_book_id int,
	param_reader_id int,
	param_min_state state)
returns void as $$
begin
	if not exists (select 1 from public.book
				   where book_id = param_book_inventory_id)
	then
		raise exception 'book doesnt exist';
	end if;

	if not exists (select 1 from public.reader
				   where reader_card_id = param_reader_id)
	then
		raise exception 'reader doesnt exist';
	end if;
	
	insert into public.booking(reader_card_id,
							   book_id,
							   min_state,
							   booking_datetime)
	values (param_reader_id,
			param_book_id,
			param_min_state,
			CURRENT_TIMESTAMP + interval '3 days');
end;
$$ language plpgsql;


-- task 12


create or replace function cancel_book_reservation(
	param_issuance_id int)
returns void as $$
begin
	delete from public.booking
	where issuance_id = param_issuance_id;
end;
$$ language plpgsql;


-- task 13


create or replace function more_modified_issue_book(
	param_book_id int,
	param_reader_id int,
	param_issuance_datetime timestamp,
	param_expected_refund_date date)
returns void as $$
begin
	if not exists (select 1 from public.book as pb
				   where pb.book_id = param_book_id)
	then 
		raise exception 'this book doesnt exist';
	end if;

	if not exists (select 1 from public.reader as pr
				   where pr.reader_id = param_reader_id)
	then
		raise exception 'this reader doesnt exist';
	end if;

	if not exists (select 1 from public.book_instance
				   where book_inventory_id = param_book_id
				   and book_status = 'in stock')
	then
		raise exception 'this book cannot be issued';
	end if;

	if exists(
		select 1 from overdue_books
		where reader_card_id = param_reader_id)
	then
		raise exception 'gde kniga lebovski???';
	end if;

	if exists (select 1 from public.booking b
			   inner join public.book_instance bi 
			       on b.book_id = bi.book_id
			   where bi.book_inventory_id = param_book_id
			   and b.reader_card_id != param_reader_id
			   and b.booking_datetimt > CURRENT_TIMESTAMP)
	then
		raise exception 'book is reserved';
	end if;
	
	insert into public.issuance(book_inventory_id,
								reader_card_id,
								issuance_datetime,
								expected_refund_date)
	values (param_book_id,
			param_reader_id,
			param_issuance_datetime,
			param_expected_refund_datetime);

	update public.book_instance 
    set book_status = 'issued' 
    where book_inventory_id = param_book_inventory_id;
end;
$$ language plpgsql;


-- task 14


create or replace function get_book_locations(
	param_book_id int)
returns table(
	book_inventory_id int,
	book_location varchar,
	book_state state)
		as $$
begin
	return query
	select bi.book_inventory_id, 
		   bi.book_location,
		   bi.book_state
	from public.book_instance bi
	inner join public.book b
		on b.book_id = bi.book_id
	where bi.book_id = param_book_id
	order by book_state;
end;
$$ language plpgsql;

select * from get_book_locations(1);


-- task 15


create view books_not_issued_or_reserved as
select 
    bi.book_id,
    bi.book_state,
    COUNT(*) AS copies_count
from public.book_instance bi
where bi.book_status = 'in stock'
group by bi.book_id, bi.book_state;


-- task 16


create view books_not_returned_for_more_than_a_year as
select
    bi.book_inventory_id
from public.issuance i
inner join public.book_instance bi 
	on i.book_inventory_id = bi.book_inventory_id
where i.actual_refund_date is null
and i.issuance_datetime < CURRENT_DATE - INTERVAL '1 year'; 


-- task 17


create table if not exists public.logs
(
	log_id serial primary key,
	log_timestamp timestamp not null,
	log_table varchar not null,
	log_info varchar not null
);


-- task 18


create or replace function log_changes()
returns trigger as $$
begin
    if TG_OP = 'INSERT' 
	then
        insert into public.logs (log_timestamp, log_table, log_info)
        values (now(), TG_TABLE_NAME, 'something inserted'); 
    elsif TG_OP = 'UPDATE' 
	then
        insert into public.logs (log_timestamp, log_table, log_info)
        values (now(), TG_TABLE_NAME, 'something updated');
    elsif TG_OP = 'DELETE' 
	then
        insert into public.logs (log_timestamp, log_table, log_info)
        values (now(), TG_TABLE_NAME, 'something deleted');
    end if;
    
    return null;
end;
$$ language plpgsql;


create trigger author_log_trigger
	after insert or update or delete on public.author
    for each row
    execute function log_changes();


create trigger publishing_house_log_trigger
    after insert or update or delete on public.publishing_house
    for each row
    execute function log_changes();


create trigger book_log_trigger
    after insert or update or delete on public.book
    for each row
    execute function log_changes();


create trigger reader_log_trigger
    after insert or update or delete on public.reader
    for each row
    execute function log_changes();


create trigger book_instance_log_trigger
    after insert or update or delete on public.book_instance
    for each row
    execute function log_changes();


create trigger issuance_log_trigger
    after insert or update or delete on public.issuance
    for each row
    execute function log_changes();


create trigger booking_log_trigger
    after insert or update or delete on public.booking
    for each row
    execute function log_changes();

update public.author
set author_first_name = 'hzzzsSDAAAFDSfh', author_last_name = 'fgsjfiodgijosd'
where author_id = 7;
