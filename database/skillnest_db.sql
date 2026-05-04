DROP DATABASE IF EXISTS skillnest;
CREATE DATABASE skillnest;
USE skillnest;

CREATE TABLE users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    role ENUM('student', 'teacher', 'admin') NOT NULL,
    status ENUM('active', 'suspended') DEFAULT 'active',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE courses (
    course_id INT AUTO_INCREMENT PRIMARY KEY,
    teacher_id INT NOT NULL,
    title VARCHAR(150) NOT NULL,
    description TEXT,
    status ENUM('pending', 'approved', 'rejected') DEFAULT 'pending',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (teacher_id) REFERENCES users(user_id)
        ON DELETE CASCADE
);

CREATE TABLE lessons (
    lesson_id INT AUTO_INCREMENT PRIMARY KEY,
    course_id INT NOT NULL,
    title VARCHAR(150) NOT NULL,
    video_url VARCHAR(255),
    content TEXT,
    lesson_order INT DEFAULT 1,

    FOREIGN KEY (course_id) REFERENCES courses(course_id)
        ON DELETE CASCADE
);

CREATE TABLE enrollments (
    enrollment_id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    course_id INT NOT NULL,
    enroll_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status ENUM('active', 'completed') DEFAULT 'active',

    FOREIGN KEY (student_id) REFERENCES users(user_id)
        ON DELETE CASCADE,

    FOREIGN KEY (course_id) REFERENCES courses(course_id)
        ON DELETE CASCADE
);

CREATE TABLE progress (
    progress_id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    course_id INT NOT NULL,
    lesson_id INT,
    score INT DEFAULT 0,
    completion_status BOOLEAN DEFAULT FALSE,
    last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,

    FOREIGN KEY (student_id) REFERENCES users(user_id)
        ON DELETE CASCADE,

    FOREIGN KEY (course_id) REFERENCES courses(course_id)
        ON DELETE CASCADE,

    FOREIGN KEY (lesson_id) REFERENCES lessons(lesson_id)
        ON DELETE SET NULL
);

CREATE TABLE quizzes (
    quiz_id INT AUTO_INCREMENT PRIMARY KEY,
    course_id INT NOT NULL,
    title VARCHAR(150) NOT NULL,
    total_marks INT DEFAULT 0,

    FOREIGN KEY (course_id) REFERENCES courses(course_id)
        ON DELETE CASCADE
);

CREATE TABLE questions (
    question_id INT AUTO_INCREMENT PRIMARY KEY,
    quiz_id INT NOT NULL,
    question_text TEXT NOT NULL,
    option_a VARCHAR(255),
    option_b VARCHAR(255),
    option_c VARCHAR(255),
    option_d VARCHAR(255),
    answer CHAR(1) NOT NULL,

    FOREIGN KEY (quiz_id) REFERENCES quizzes(quiz_id)
        ON DELETE CASCADE
);

CREATE TABLE grades (
    grade_id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    quiz_id INT NOT NULL,
    marks_obtained INT DEFAULT 0,
    published_status BOOLEAN DEFAULT FALSE,
    published_at TIMESTAMP NULL,

    FOREIGN KEY (student_id) REFERENCES users(user_id)
        ON DELETE CASCADE,

    FOREIGN KEY (quiz_id) REFERENCES quizzes(quiz_id)
        ON DELETE CASCADE
);

CREATE TABLE announcements (
    announcement_id INT AUTO_INCREMENT PRIMARY KEY,
    course_id INT NOT NULL,
    teacher_id INT NOT NULL,
    title VARCHAR(150) NOT NULL,
    message TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (course_id) REFERENCES courses(course_id)
        ON DELETE CASCADE,

    FOREIGN KEY (teacher_id) REFERENCES users(user_id)
        ON DELETE CASCADE
);

INSERT INTO users(name, email, password, role)
VALUES
('Admin User', 'admin@skillnest.com', 'admin123', 'admin'),
('Teacher User', 'teacher@skillnest.com', 'teacher123', 'teacher'),
('Student User', 'student@skillnest.com', 'student123', 'student');

INSERT INTO courses(teacher_id, title, description, status)
VALUES
(2, 'Introduction to C++', 'Beginner-friendly C++ course', 'approved');

INSERT INTO lessons(course_id, title, video_url, content, lesson_order)
VALUES
(1, 'What is C++?', 'https://example.com/video1', 'C++ is a programming language.', 1);

INSERT INTO enrollments(student_id, course_id)
VALUES
(3, 1);

INSERT INTO progress(student_id, course_id, lesson_id, score, completion_status)
VALUES
(3, 1, 1, 80, TRUE);