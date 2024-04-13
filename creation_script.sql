create table users(id INTEGER PRIMARY KEY AUTOINCREMENT,
				username TEXT NOT NULL,
				password TEXT NOT NULL);

create TABLE categories(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT NOT NULL
);

				
create table articles(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	title TEXT NOT NULL,
	description TEXT NOT NULL,
	price REAL NOT NULL,
	status TEXT NOT NULL DEFAULT 'unsold',
	pub_date date NOT NULL DEFAULT CURRENT_DATE,
	owner_id INTEGER NOT NULL,
	category TEXT NOT NULL,
	FOREIGN KEY (owner_id) REFERENCES users(id)
);

create TABLE purchase_history(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	ad_id INTEGER NOT NULL,
	user_id INTEGER NOT NULL,
	FOREIGN KEY (ad_id) REFERENCES articles(id),
	FOREIGN KEY (user_id) REFERENCES users(id)
);

drop table purchase_history;
drop table articles;

CREATE UNIQUE INDEX unique_category on categories(name);


INSERT into articles(title, description, price, owner_id, category) values ('cel mai titlu', 'cea mai descriere', 14.5, 3, 'categorie');
select * from users;

select id, title, description, price, status, owner_id, category from articles where owner_id = 1;

