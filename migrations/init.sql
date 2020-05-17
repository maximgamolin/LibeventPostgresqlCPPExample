CREATE TABLE IF NOT EXISTS author(
    id SERIAL PRIMARY KEY NOT NULL,
    login CHARACTER VARYING(50) UNIQUE NOT NULL,
    password CHARACTER VARYING(32) NOT NULL,
    first_name CHARACTER VARYING(32) NULL,
    last_name CHARACTER VARYING(32) NULL,
    registered_at timestamp not null default CURRENT_TIMESTAMP
);
CREATE TABLE IF NOT EXISTS session(
    ID SERIAL PRIMARY KEY NOT NULL,
    token CHARACTER VARYING(255) UNIQUE NOT NULL,
    created_at timestamp not null default CURRENT_TIMESTAMP,
    user_id INTEGER UNIQUE NOT NULL REFERENCES author(id) ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS tweet(
    ID SERIAL PRIMARY KEY NOT NULL,
    title CHARACTER VARYING(255),
    body TEXT,
    created_at timestamp not null default CURRENT_TIMESTAMP,
    user_id INTEGER NOT NULL REFERENCES author(id) ON DELETE CASCADE
);