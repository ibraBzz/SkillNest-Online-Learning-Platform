CREATE DATABASE  IF NOT EXISTS `skillnest` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `skillnest`;
-- MySQL dump 10.13  Distrib 8.0.46, for Win64 (x86_64)
--
-- Host: localhost    Database: skillnest
-- ------------------------------------------------------
-- Server version	8.0.46

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `announcements`
--

DROP TABLE IF EXISTS `announcements`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `announcements` (
  `id` int NOT NULL AUTO_INCREMENT,
  `course_id` int DEFAULT NULL,
  `course_title` varchar(255) DEFAULT NULL,
  `message` varchar(255) DEFAULT NULL,
  `posted_by` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `course_id` (`course_id`),
  CONSTRAINT `announcements_ibfk_1` FOREIGN KEY (`course_id`) REFERENCES `courses` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `announcements`
--

LOCK TABLES `announcements` WRITE;
/*!40000 ALTER TABLE `announcements` DISABLE KEYS */;
INSERT INTO `announcements` VALUES (1,1,'CSE332','Tomorrow%27s class has been cancelled','tanjila@skillnest.com'),(2,8,'CSE332','quiz cancelled','tanjila@skillnest.com'),(3,8,'CSE332','quiz next week','tanjila@skillnest.com'),(4,8,'CSE332','class cancelled','tanjila@skillnest.com'),(5,23,'CSE373','Quiz 2 cancelled','iqtidar@skillnest.com');
/*!40000 ALTER TABLE `announcements` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `courses`
--

DROP TABLE IF EXISTS `courses`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `courses` (
  `id` int NOT NULL AUTO_INCREMENT,
  `title` varchar(100) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `teacher_name` varchar(100) DEFAULT NULL,
  `approved` tinyint(1) DEFAULT '0',
  `teacher_email` varchar(255) DEFAULT NULL,
  `feature_flags` varchar(100) DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `courses`
--

LOCK TABLES `courses` WRITE;
/*!40000 ALTER TABLE `courses` DISABLE KEYS */;
INSERT INTO `courses` VALUES (1,'cse327','Software engineering','Iqtidar Newaz',1,'iqtidar@skillnest.com',''),(2,'Test proxy course','Pending approval demo','ibra',1,NULL,''),(3,'CSE323','Operating Systems','Salman Shamil',1,NULL,''),(4,'CSE332','Computer Architecture','Tanjila Farah',1,NULL,''),(8,'CSE332','Computer Architecture','Tanjila Farah',1,'tanjila@skillnest.com',''),(9,'CSE323','Operating Systems','Salman Shamil',1,'salman@skillnest.com',''),(14,'CSE334','Python learning','Iqtidar Newaz',0,'iqtidar@skillnest.com',''),(15,'CSE225','Data Structures','Iqtidar Newaz',0,'iqtidar@skillnest.com',''),(16,'CSE231','Digital Logic Design','Tanjila Farah',1,'tanjila@skillnest.com',''),(17,'EEE111','Analog Circuits','Tanjila Farah',0,'tanjila@skillnest.com',''),(18,'CSE445','Cybersecurity','Tanjila Farah',0,'tanjila@skillnest.com',''),(19,'EEE141','Electrical Circuit','Tanjila Farah',1,'tanjila@skillnest.com',''),(20,'CSE332','Computer Architecture','Tanjila Farah',0,'tanjila@skillnest.com',''),(21,'CSE311','Database','Abdus Salam',1,'salam@teacher.com',''),(22,'CSE115','Programming','Tanjila Farah',1,'tanjila@skillnest.com',''),(23,'CSE373','Algorithm','Iqtidar Newaz',1,'iqtidar@skillnest.com','');
/*!40000 ALTER TABLE `courses` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enrollments`
--

DROP TABLE IF EXISTS `enrollments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `enrollments` (
  `id` int NOT NULL AUTO_INCREMENT,
  `student_email` varchar(100) DEFAULT NULL,
  `course_id` int DEFAULT NULL,
  `status` varchar(20) DEFAULT 'pending',
  PRIMARY KEY (`id`),
  KEY `course_id` (`course_id`),
  CONSTRAINT `enrollments_ibfk_1` FOREIGN KEY (`course_id`) REFERENCES `courses` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enrollments`
--

LOCK TABLES `enrollments` WRITE;
/*!40000 ALTER TABLE `enrollments` DISABLE KEYS */;
INSERT INTO `enrollments` VALUES (1,'ibra@student.com',1,'pending'),(2,'ibra@student.com',4,'pending'),(3,'urbana@student.com',4,'pending'),(4,'nuha@student.com',4,'pending'),(5,'shafin@student.com',4,'pending'),(7,'nuha@student.com',8,'approved'),(8,'shafin@student.com',8,'approved'),(9,'urbana@student.com',8,'approved'),(10,'ibra@student.com',16,'approved'),(12,'ibra@student.com',2,'pending'),(13,'ibra@student.com',21,'approved'),(14,'ibra@student.com',19,'approved'),(15,'shafin@student.com',16,'approved'),(16,'shafin@student.com',19,'approved'),(17,'urbana@student.com',16,'approved'),(18,'ruhia@student.com',8,'approved'),(19,'ruhia@student.com',19,'approved'),(20,'nuha@student.com',16,'approved'),(21,'nuha@student.com',19,'approved'),(22,'ibra@student.com',8,'pending'),(23,'ibra@student.com',9,'pending'),(24,'ibra@student.com',23,'approved'),(25,'nuha@student.com',23,'approved'),(26,'urbana@student.com',23,'approved');
/*!40000 ALTER TABLE `enrollments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `grades`
--

DROP TABLE IF EXISTS `grades`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `grades` (
  `id` int NOT NULL AUTO_INCREMENT,
  `student_email` varchar(100) DEFAULT NULL,
  `course_id` int DEFAULT NULL,
  `marks` int DEFAULT NULL,
  `letter_grade` varchar(5) DEFAULT NULL,
  `published` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `grades`
--

LOCK TABLES `grades` WRITE;
/*!40000 ALTER TABLE `grades` DISABLE KEYS */;
INSERT INTO `grades` VALUES (1,'ibra@student.com',1,10,'A',1),(2,'ibra@student.com',8,30,'A',1),(3,'ibra@student.com',9,8,'B-',1);
/*!40000 ALTER TABLE `grades` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `lessons`
--

DROP TABLE IF EXISTS `lessons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `lessons` (
  `id` int NOT NULL AUTO_INCREMENT,
  `course_id` int DEFAULT NULL,
  `title` varchar(100) DEFAULT NULL,
  `video_url` varchar(255) DEFAULT NULL,
  `content` text,
  `teacher_email` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `course_id` (`course_id`),
  CONSTRAINT `lessons_ibfk_1` FOREIGN KEY (`course_id`) REFERENCES `courses` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `lessons`
--

LOCK TABLES `lessons` WRITE;
/*!40000 ALTER TABLE `lessons` DISABLE KEYS */;
INSERT INTO `lessons` VALUES (1,1,'Design pattern','lesson1_video','Observer pattern',NULL),(2,19,'Kvl','234yt.com/vd.mp4','Kirchoff%27s Law','tanjila@skillnest.com'),(3,23,'MergeSort','234yt.com/vd.hh','sorting','iqtidar@skillnest.com');
/*!40000 ALTER TABLE `lessons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `notifications`
--

DROP TABLE IF EXISTS `notifications`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `notifications` (
  `id` int NOT NULL AUTO_INCREMENT,
  `email` varchar(255) DEFAULT NULL,
  `message` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `notifications`
--

LOCK TABLES `notifications` WRITE;
/*!40000 ALTER TABLE `notifications` DISABLE KEYS */;
INSERT INTO `notifications` VALUES (1,'ibra@student.com','quiz cancelled: 2'),(2,'ibra@student.com','&message=class cancelled: essage=&message=class cancelled'),(3,'ibra@student.com','CSE332: quiz cancelled'),(4,'nuha@student.com','CSE332: quiz cancelled'),(5,'shafin@student.com','CSE332: quiz cancelled'),(6,'urbana@student.com','CSE332: quiz cancelled'),(7,'ibra@student.com','CSE332: quiz next week'),(8,'urbana@student.com','CSE332: quiz next week'),(9,'shafin@student.com','CSE332: quiz next week'),(10,'nuha@student.com','CSE332: quiz next week'),(11,'ibra@student.com','CSE332: class cancelled'),(12,'nuha@student.com','CSE332: class cancelled'),(13,'shafin@student.com','CSE332: class cancelled'),(14,'urbana@student.com','CSE332: class cancelled'),(15,'ibra@student.com','CSE373: Quiz 2 cancelled'),(16,'nuha@student.com','CSE373: Quiz 2 cancelled'),(17,'urbana@student.com','CSE373: Quiz 2 cancelled');
/*!40000 ALTER TABLE `notifications` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `progress`
--

DROP TABLE IF EXISTS `progress`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `progress` (
  `id` int NOT NULL AUTO_INCREMENT,
  `student_email` varchar(100) DEFAULT NULL,
  `course_id` int DEFAULT NULL,
  `progress_percent` int DEFAULT '0',
  `completed` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `progress`
--

LOCK TABLES `progress` WRITE;
/*!40000 ALTER TABLE `progress` DISABLE KEYS */;
/*!40000 ALTER TABLE `progress` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quiz_results`
--

DROP TABLE IF EXISTS `quiz_results`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `quiz_results` (
  `id` int NOT NULL AUTO_INCREMENT,
  `student_email` varchar(100) DEFAULT NULL,
  `quiz_id` int DEFAULT NULL,
  `score` int DEFAULT NULL,
  `graded` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `quiz_id` (`quiz_id`),
  CONSTRAINT `quiz_results_ibfk_1` FOREIGN KEY (`quiz_id`) REFERENCES `quizzes` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quiz_results`
--

LOCK TABLES `quiz_results` WRITE;
/*!40000 ALTER TABLE `quiz_results` DISABLE KEYS */;
INSERT INTO `quiz_results` VALUES (1,'ibra@student.com',1,20,1),(2,'ibra@student.com',1,12,1),(3,'ibra@student.com',1,NULL,0),(4,'ibra@student.com',4,NULL,0),(5,'ibra@student.com',4,NULL,0),(6,'ibra@student.com',5,30,1),(7,'ibra@student.com',3,NULL,0),(8,'ibra@student.com',7,NULL,0),(9,'ibra@student.com',2,NULL,0),(10,'ibra@student.com',8,8,1);
/*!40000 ALTER TABLE `quiz_results` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `quizzes`
--

DROP TABLE IF EXISTS `quizzes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `quizzes` (
  `id` int NOT NULL AUTO_INCREMENT,
  `course_id` int DEFAULT NULL,
  `title` varchar(100) DEFAULT NULL,
  `quiz_type` varchar(50) DEFAULT NULL,
  `total_marks` int DEFAULT NULL,
  `teacher_name` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `course_id` (`course_id`),
  CONSTRAINT `quizzes_ibfk_1` FOREIGN KEY (`course_id`) REFERENCES `courses` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `quizzes`
--

LOCK TABLES `quizzes` WRITE;
/*!40000 ALTER TABLE `quizzes` DISABLE KEYS */;
INSERT INTO `quizzes` VALUES (1,1,'cse327','MCQ',10,'Iqtidar Newaz'),(2,1,'Design pattern','MCQ',10,'Iqtidar Newaz'),(3,1,'mid','written',100,'Tanjila Farah'),(4,8,'Quiz 2','mcq',100,'Tanjila Farah'),(5,8,'Mips','written',20,'Tanjila Farah'),(6,22,'variables','mcq',20,'Tanjila Farah'),(7,8,'Final','written',40,'Tanjila Farah'),(8,9,'Segmentation','mcq',10,'Salman Shamil'),(9,23,'MergeSort','written',20,'Iqtidar Newaz'),(10,9,'Paging','written',20,'Salman Shamil');
/*!40000 ALTER TABLE `quizzes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `student_features`
--

DROP TABLE IF EXISTS `student_features`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `student_features` (
  `id` int NOT NULL AUTO_INCREMENT,
  `student_email` varchar(255) DEFAULT NULL,
  `course_title` varchar(255) DEFAULT NULL,
  `certificate` tinyint(1) DEFAULT '0',
  `notes` tinyint(1) DEFAULT '0',
  `videos` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `student_features`
--

LOCK TABLES `student_features` WRITE;
/*!40000 ALTER TABLE `student_features` DISABLE KEYS */;
INSERT INTO `student_features` VALUES (1,'ibra@student.com','CSE231&basic=on&notes=on',0,1,0),(2,'ibra@student.com','CSE311&basic=on&notes=on',0,1,0),(3,'ibra@student.com','CSE231',0,1,1),(4,'ibra@student.com','CSE311',0,1,1),(5,'ibra@student.com','CSE373',1,1,0),(6,'ibra@student.com','EEE141',1,0,0);
/*!40000 ALTER TABLE `student_features` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(100) DEFAULT NULL,
  `email` varchar(100) DEFAULT NULL,
  `password` varchar(100) DEFAULT NULL,
  `role` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `email` (`email`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'Admin User','admin@skillnest.com','1234','admin'),(2,'Teacher User','teacher@skillnest.com','1234','teacher'),(3,'Student User','student@skillnest.com','1234','student'),(4,'ibra','ibra@student.com','1234','student'),(5,'Tanjila Farah','tanjila@skillnest.com','1234','teacher'),(6,'Salman Shamil','salman@skillnest.com','1234','teacher'),(7,'Iqtidar Newaz','iqtidar@skillnest.com','1234','teacher'),(8,'Urbana','urbana@student.com','1234','student'),(9,'Shafin','shafin@student.com','1234','student'),(10,'Nuha','nuha@student.com','1234','student'),(11,'Ruhia','ruhia@student.com','1234','student'),(12,'Abdus Salam','salam@teacher.com','1234','teacher'),(13,'Admin2','admin2@skillnest.com','1234','admin'),(14,'sophie','sophie@skillnest.com','1234','teacher');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-05-04  7:43:20
