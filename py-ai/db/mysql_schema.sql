
CREATE DATABASE IF NOT EXISTS cdm_memory;
USE cdm_memory;

CREATE TABLE sessions(

    session_id CHAR(32) PRIMARY KEY,
    user_id VARCHAR(64),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX(user_id),
    INDEX(created_at)

)ENGINE=InnoDB;

CREATE TABLE memory_entries(

    memory_id BIGINT AUTO_INCREMENT,
    session_id CHAR(32),
    section VARCHAR(32),
    content TEXT,
    last_access TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (memory_id, session_id)

)ENGINE=InnoDB
PARTITION BY KEY(session_id) PARTITIONS 32;

