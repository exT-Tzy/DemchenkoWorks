--task1

create database t01_library if not exists;

--task2

create table if not exists public.author
(
	author_id serial primary key,
	author_first_name varchar(50) not null,
	author_last_name varchar(50) not null
);

--task3

create table if not exists public.publishing_house
(
	publishing_house_id serial primary key,
	publishing_house_name varchar(100) not null,
	publishing_house_city varchar(100) not null
);

--task4

create table if not exists public.book
(
	book_id serial primary key,
	book_title varchar(100) not null,
	author_id int not null,
	foreign key (author_id) references public.author(author_id),
	publishing_house_id int not null,
	foreign key (publishing_house_id) references public.publishing_house(publishing_house_id),
	book_publication int not null,
	book_publication_year int not null,
	book_circulation int not null
);

--task5

create type gender as enum(/*'ZZZhenshina', 'muZZZhchina')*/ 'male', 'female');

create table if not exists public.reader
(
	reader_card_id int primary key,
	reader_first_name varchar(50) not null,
	reader_last_name varchar(50) not null,
	reader_date_of_birth date not null,
	reader_gender gender not null,
	reader_date_of_registration date not null
);

--task6

create type state as enum('perfect', 'good', 'satisfactory', 'dilapidated', 'lost');

create type status as enum('in stock', 'issued', 'booked');

create table if not exists public.book_instance
(
	book_inventory_id int primary key,
	book_id int not null,
	foreign key (book_id) references public.book(book_id),
	book_state state not null,
	book_status status not null,
	book_location varchar(50) check (book_location ~'^/\d+/\d+/\d+$')
);

--task7

create table if not exists public.issuance
(
	reader_card_id int not null,
	foreign key (reader_card_id) references public.reader(reader_card_id),
	book_inventory_id int not null,
	foreign key (book_inventory_id) references public.book_instance (book_inventory_id),
	primary key (reader_card_id, book_inventory_id),
	issuance_datetime timestamp(0) not null,
	expected_refund_date date not null,
	actual_refund_date date
);

--task8

create table if not exists public.booking
(
	issuance_id serial primary key,
	reader_card_id int not null,
	foreign key (reader_card_id) references public.reader (reader_card_id),
	book_id int not null,
	foreign key (book_id) references public.book (book_id),
	min_state state not null,
	booking_datetime timestamp(0)
);





