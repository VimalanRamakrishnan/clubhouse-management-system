-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 26, 2025 at 06:37 AM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";

CREATE DATABASE IF NOT EXISTS `restoranaliff`
  CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE `restoranaliff`;


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `restoranaliff`
--

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

CREATE TABLE `admin` (
  `Username` varchar(255) NOT NULL,
  `Password` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`Username`, `Password`) VALUES
('demo_admin', 'Admin@123');

-- --------------------------------------------------------

--
-- Table structure for table `booking`
--

CREATE TABLE `booking` (
  `book_id` int(11) NOT NULL,
  `fasi_id` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `cust_id` int(11) NOT NULL,
  `date` date NOT NULL,
  `pax` int(11) NOT NULL,
  `entry_time` time NOT NULL,
  `exit_time` time NOT NULL,
  `duration` varchar(255) NOT NULL,
  `amount` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `booking`
--

INSERT INTO `booking` (`book_id`, `fasi_id`, `Name`, `cust_id`, `date`, `pax`, `entry_time`, `exit_time`, `duration`, `amount`) VALUES
(1, 1, 'Demo Resident', 1, '2026-01-15', 2, '10:00:00', '12:00:00', '2.00', '37.40'),
(2, 4, 'Demo Visitor', 2, '2026-01-16', 4, '14:00:00', '15:30:00', '1.50', '30.00');

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

CREATE TABLE `customer` (
  `ID` int(11) NOT NULL,
  `Name` varchar(100) DEFAULT NULL,
  `Phone` varchar(20) DEFAULT NULL,
  `Add` varchar(255) DEFAULT NULL,
  `Username` varchar(50) DEFAULT NULL,
  `Password` varchar(255) DEFAULT NULL,
  `status` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `customer`
--

INSERT INTO `customer` (`ID`, `Name`, `Phone`, `Add`, `Username`, `Password`, `status`) VALUES
(1, 'Demo Resident', '0100000001', 'Demo Address', 'resident_demo', 'Demo@123', 1),
(2, 'Demo Visitor', '0100000002', 'Demo Address', 'visitor_demo', 'Demo@123', 0);

-- --------------------------------------------------------

--
-- Table structure for table `facilities`
--

CREATE TABLE `facilities` (
  `facility_id` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `status` enum('Available','Under Maintenance','Closed') DEFAULT 'Available',
  `max_capacity` int(11) DEFAULT NULL,
  `hourly_rate` decimal(10,2) DEFAULT NULL,
  `opening_time` time DEFAULT NULL,
  `closing_time` time DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `facilities`
--

INSERT INTO `facilities` (`facility_id`, `name`, `status`, `max_capacity`, `hourly_rate`, `opening_time`, `closing_time`) VALUES
(1, 'Swimming Pool', 'Available', 40, 22.00, '10:00:00', '19:00:00'),
(4, 'Badminton', 'Available', 50, 20.00, '09:00:00', '23:30:00'),
(5, 'Futsal', 'Available', 10, 50.00, '09:00:00', '23:30:00'),
(6, 'Function Hall', 'Available', 100, 200.00, '10:00:00', '23:00:00'),
(9, 'Gym', 'Available', 40, 7.00, '09:00:00', '23:30:00'),
(10, 'TableTennis', 'Closed', 10, 24.00, '10:00:00', '21:00:00'),
(11, 'Karoake', 'Available', 35, 55.00, '10:00:00', '23:59:00'),
(13, 'BeautyParlor', 'Closed', 15, 30.00, '09:00:00', '23:00:00'),
(16, 'PickleBall', 'Available', 25, 40.00, '10:00:00', '22:00:00'),
(21, 'GameRoom', 'Available', 25, 12.00, '10:00:00', '23:00:00');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `booking`
--
ALTER TABLE `booking`
  ADD PRIMARY KEY (`book_id`);

--
-- Indexes for table `customer`
--
ALTER TABLE `customer`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `Username` (`Username`);

--
-- Indexes for table `facilities`
--
ALTER TABLE `facilities`
  ADD PRIMARY KEY (`facility_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `booking`
--
ALTER TABLE `booking`
  MODIFY `book_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT for table `customer`
--
ALTER TABLE `customer`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT for table `facilities`
--
ALTER TABLE `facilities`
  MODIFY `facility_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=26;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
