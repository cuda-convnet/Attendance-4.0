-- phpMyAdmin SQL Dump
-- version 4.5.1
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Jul 16, 2016 at 06:12 PM
-- Server version: 10.1.9-MariaDB
-- PHP Version: 5.6.15

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `ligerbots_attendance`
--

-- --------------------------------------------------------

--
-- Table structure for table `calendar`
--

CREATE TABLE `calendar` (
  `id` int(11) NOT NULL,
  `userid` int(11) NOT NULL,
  `start` int(11) NOT NULL,
  `end` int(11) NOT NULL,
  `special` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `calendar`
--

INSERT INTO `calendar` (`id`, `userid`, `start`, `end`, `special`) VALUES
(10, 1, 1468472632, 1468472634, '{"adminEventLog":["0",38,39]}'),
(11, 1, 1468472636, 1468472637, '{"adminEventLog":["0",40,41]}'),
(12, 1, 1468472638, 1468472638, '{"adminEventLog":["0",42,43]}'),
(13, 1, 1468474052, 1468474068, '{"adminEventLog":["0",44,45]}'),
(14, 1, 1468507478, 1468507478, '{"adminEventLog":["0",46,47]}'),
(15, 1, 1468516060, 1468516060, '{"adminEventLog":["0",48,49]}'),
(16, 1, 1468516196, 1468516196, '{"adminEventLog":["0",50,51]}');

-- --------------------------------------------------------

--
-- Table structure for table `events`
--

CREATE TABLE `events` (
  `id` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `reference` text NOT NULL,
  `who` text NOT NULL,
  `time` bigint(20) UNSIGNED NOT NULL,
  `notes` text NOT NULL,
  `pvalue` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `events`
--

INSERT INTO `events` (`id`, `type`, `reference`, `who`, `time`, `notes`, `pvalue`) VALUES
(38, 4, '10', 'admin', 1468472632, '', ''),
(39, 3, '10', 'admin', 1468472634, '', ''),
(40, 4, '11', 'admin', 1468472636, '', ''),
(41, 3, '11', 'admin', 1468472637, '', ''),
(42, 4, '12', 'admin', 1468472638, '', ''),
(43, 3, '12', 'admin', 1468472638, '', ''),
(44, 4, '13', 'admin', 1468474052, '', ''),
(45, 1, '13', 'admin', 1468474068, '', ''),
(46, 4, '14', 'admin', 1468507478, '', ''),
(47, 2, '', 'admin', 1468515998, '', '{}'),
(48, 4, '15', 'admin', 1468516060, '', '{}'),
(49, 2, '', 'admin', 1468516075, '', '{}'),
(50, 4, '16', 'admin', 1468516196, '', '{}'),
(51, 2, '16', 'admin', 1468516201, '', '{}');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `fname` text NOT NULL,
  `lname` text NOT NULL,
  `email` text NOT NULL,
  `pin` text NOT NULL,
  `rfid` text NOT NULL,
  `lastsignin` bigint(11) NOT NULL,
  `totaltime` int(11) NOT NULL,
  `signedin` tinyint(1) NOT NULL,
  `username` text NOT NULL,
  `passhash` text NOT NULL,
  `permissions` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`id`, `fname`, `lname`, `email`, `pin`, `rfid`, `lastsignin`, `totaltime`, `signedin`, `username`, `passhash`, `permissions`) VALUES
(1, 'Nicholas', 'Hollander', '', '8888', 'XXXXXXXX', 1468461803560, 0, 1, 'admin', '$2y$10$KaPB2/H3nD00WAsd6IdPzeFD9uU9iA.Zv3nvTxeCngk/jvERxpdrq', '*');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `calendar`
--
ALTER TABLE `calendar`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `events`
--
ALTER TABLE `events`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `calendar`
--
ALTER TABLE `calendar`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=17;
--
-- AUTO_INCREMENT for table `events`
--
ALTER TABLE `events`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=52;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
