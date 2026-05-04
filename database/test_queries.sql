USE skillnest;

SELECT * FROM users;

SELECT * FROM courses;

SELECT 
    users.name AS student_name,
    courses.title AS course_title,
    enrollments.status
FROM enrollments
JOIN users ON enrollments.student_id = users.user_id
JOIN courses ON enrollments.course_id = courses.course_id;

SELECT 
    users.name,
    courses.title,
    progress.score,
    progress.completion_status
FROM progress
JOIN users ON progress.student_id = users.user_id
JOIN courses ON progress.course_id = courses.course_id;