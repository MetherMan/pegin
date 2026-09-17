/*M!999999\- enable the sandbox mode */ 

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `LoginAccount` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */;

USE `LoginAccount`;
DROP TABLE IF EXISTS `Rebirth`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `Rebirth` (
  `mb_no` int(11) NOT NULL AUTO_INCREMENT,
  `mb_ip` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `mb_id` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  `rebirths` varchar(255) CHARACTER SET latin1 COLLATE latin1_swedish_ci DEFAULT NULL,
  UNIQUE KEY `mb_id` (`mb_id`),
  KEY `mb_no` (`mb_no`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 COLLATE=utf8mb3_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `g4_member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `g4_member` (
  `mb_no` int(11) NOT NULL AUTO_INCREMENT,
  `mb_ip` varchar(255) NOT NULL DEFAULT '',
  `mb_id` varchar(255) NOT NULL DEFAULT '',
  `mb_password` varchar(255) NOT NULL DEFAULT '',
  `mb_name` varchar(255) NOT NULL DEFAULT '',
  `mb_nick` varchar(255) NOT NULL DEFAULT '',
  `mb_nick_date` date NOT NULL DEFAULT '0000-00-00',
  `mb_email` varchar(255) NOT NULL DEFAULT '',
  `mb_sex` char(6) NOT NULL DEFAULT '',
  UNIQUE KEY `mb_id` (`mb_id`),
  KEY `mb_no` (`mb_no`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `LAQIA_GAMEDB` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */;

USE `LAQIA_GAMEDB`;
DROP TABLE IF EXISTS `BADUser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `BADUser` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `id` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `type` int(11) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `BillItemInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `BillItemInfo` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` tinyint(4) NOT NULL DEFAULT 0,
  `effect1` int(11) NOT NULL DEFAULT 0,
  `effect2` int(11) NOT NULL DEFAULT 0,
  `effect3` int(11) NOT NULL DEFAULT 0,
  `effect4` int(11) NOT NULL DEFAULT 0,
  `effect5` int(11) NOT NULL DEFAULT 0,
  `effect6` int(11) NOT NULL DEFAULT 0,
  `effect7` int(11) NOT NULL DEFAULT 0,
  `effect8` int(11) NOT NULL DEFAULT 0,
  `effect9` int(11) NOT NULL DEFAULT 0,
  `effect10` int(11) NOT NULL DEFAULT 0,
  `effect11` int(11) NOT NULL DEFAULT 0,
  `effect12` int(11) NOT NULL DEFAULT 0,
  `effect13` int(11) NOT NULL DEFAULT 0,
  `effect14` int(11) NOT NULL DEFAULT 0,
  `effect15` int(11) NOT NULL DEFAULT 0,
  `effect16` int(11) NOT NULL DEFAULT 0,
  `effect17` int(11) NOT NULL DEFAULT 0,
  `effect18` int(11) NOT NULL DEFAULT 0,
  `effect19` int(11) NOT NULL DEFAULT 0,
  `effect20` int(11) NOT NULL DEFAULT 0,
  `effect21` int(11) NOT NULL DEFAULT 0,
  `effect22` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `ownerID` (`ownerID`,`ownerPos`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `CreateLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `CreateLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `id` varchar(100) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `name` varchar(100) NOT NULL DEFAULT '',
  `charPos` int(11) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `name` (`name`),
  KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `DELUserTable`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `DELUserTable` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `id` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `name` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `money` int(11) NOT NULL DEFAULT 0,
  `sex` tinyint(1) NOT NULL DEFAULT 0,
  `mapNum` int(11) NOT NULL DEFAULT 0,
  `posX` int(11) NOT NULL DEFAULT 0,
  `posY` int(11) NOT NULL DEFAULT 0,
  `hp` int(11) NOT NULL DEFAULT 0,
  `mp` int(11) NOT NULL DEFAULT 0,
  `str_point` int(11) NOT NULL DEFAULT 0,
  `int_point` int(11) NOT NULL DEFAULT 0,
  `dex_point` int(11) NOT NULL DEFAULT 0,
  `skill_exp0` int(11) NOT NULL DEFAULT 0,
  `skill_exp1` int(11) NOT NULL DEFAULT 0,
  `skill_exp2` int(11) NOT NULL DEFAULT 0,
  `skill_exp3` int(11) NOT NULL DEFAULT 0,
  `skill_level0` int(11) NOT NULL DEFAULT 0,
  `skill_level1` int(11) NOT NULL DEFAULT 0,
  `skill_level2` int(11) NOT NULL DEFAULT 0,
  `skill_level3` int(11) NOT NULL DEFAULT 0,
  `last_login` int(11) NOT NULL DEFAULT 0,
  `last_logout` int(11) NOT NULL DEFAULT 0,
  `charPos` int(4) NOT NULL DEFAULT 0,
  `headStyle` int(6) NOT NULL DEFAULT 0,
  `faceStyle` int(6) NOT NULL DEFAULT 0,
  `equipHead` int(11) NOT NULL DEFAULT 0,
  `equipJacket` int(11) NOT NULL DEFAULT 0,
  `equipPants` int(11) NOT NULL DEFAULT 0,
  `equipFoot` int(11) NOT NULL DEFAULT 0,
  `equipHand` int(11) NOT NULL DEFAULT 0,
  `str_exp` int(11) NOT NULL DEFAULT 0,
  `intelli_exp` int(11) NOT NULL DEFAULT 0,
  `dex_exp` int(11) NOT NULL DEFAULT 0,
  `max_hp` int(11) NOT NULL DEFAULT 0,
  `max_mp` int(11) NOT NULL DEFAULT 0,
  `mercType` int(11) NOT NULL DEFAULT 0,
  `mercLevel` int(11) NOT NULL DEFAULT 0,
  `mercExp` int(11) NOT NULL DEFAULT 0,
  `posLayer` int(11) NOT NULL DEFAULT 0,
  `cha` int(11) NOT NULL DEFAULT 0,
  `mapKind` int(11) NOT NULL DEFAULT 0,
  `str_add` int(11) NOT NULL DEFAULT 0,
  `int_add` int(11) NOT NULL DEFAULT 0,
  `dex_add` int(11) NOT NULL DEFAULT 0,
  `guildNo` int(11) NOT NULL DEFAULT 0,
  `guildLevel` int(11) NOT NULL DEFAULT 0,
  `prevMapNum` int(11) NOT NULL DEFAULT 0,
  `prevPosX` int(11) NOT NULL DEFAULT 0,
  `prevPosY` int(11) NOT NULL DEFAULT 0,
  `lastSkill` int(11) NOT NULL DEFAULT 0,
  `isNewChar` tinyint(4) NOT NULL DEFAULT 0,
  `charClass` tinyint(4) NOT NULL DEFAULT 0,
  `tempGuild` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `id` (`id`),
  KEY `name` (`name`),
  KEY `charPos` (`charPos`),
  KEY `guildNo` (`guildNo`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `DeleteLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `DeleteLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `id` varchar(100) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `charPos` int(11) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `EquipItems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `EquipItems` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `itemNum` int(11) NOT NULL DEFAULT 0,
  `equipPos` int(11) NOT NULL DEFAULT 0,
  `life` int(11) NOT NULL DEFAULT 0,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` int(11) NOT NULL DEFAULT 0,
  `serial` varchar(25) NOT NULL DEFAULT '',
  PRIMARY KEY (`idx`),
  KEY `serial` (`serial`),
  KEY `ownerInfo` (`ownerID`,`ownerPos`),
  KEY `pos` (`equipPos`),
  KEY `itemNum` (`itemNum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `GuildData`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `GuildData` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `guildNo` int(11) NOT NULL DEFAULT 0,
  `masterName` varchar(100) NOT NULL DEFAULT '',
  `memberCnt` int(11) NOT NULL DEFAULT 0,
  `guildType` tinyint(4) NOT NULL DEFAULT 0,
  `guildName` varchar(50) NOT NULL DEFAULT '',
  `guildMark` int(11) NOT NULL DEFAULT 0,
  `guildPoint` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `guildNo` (`guildNo`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `InvenItems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `InvenItems` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `itemNum` int(11) NOT NULL DEFAULT 0,
  `invenPage` int(11) NOT NULL DEFAULT 0,
  `invenX` int(11) NOT NULL DEFAULT 0,
  `invenY` int(11) NOT NULL DEFAULT 0,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` int(11) NOT NULL DEFAULT 0,
  `life` int(11) NOT NULL DEFAULT 0,
  `exValue1` int(11) NOT NULL DEFAULT 0,
  `exValue2` int(11) NOT NULL DEFAULT 0,
  `exValue3` int(11) NOT NULL DEFAULT 0,
  `serial` varchar(25) NOT NULL DEFAULT '',
  PRIMARY KEY (`idx`),
  KEY `owner` (`ownerID`,`ownerPos`),
  KEY `serial` (`serial`),
  KEY `invenPos` (`invenPage`,`invenX`,`invenY`),
  KEY `itemNum` (`itemNum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `MAPOwnerGuild`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `MAPOwnerGuild` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `mapNum` int(11) NOT NULL DEFAULT 0,
  `guildNum` int(11) NOT NULL DEFAULT 0,
  `taxValue` int(11) NOT NULL DEFAULT 0,
  `taxBank` bigint(20) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `num` (`mapNum`,`guildNum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserCashMoney`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserCashMoney` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(100) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `amount` int(20) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `id` (`ownerID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserFriend`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserFriend` (
  `idx` bigint(20) NOT NULL DEFAULT 0,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` tinyint(4) NOT NULL DEFAULT 0,
  `FriendName` varchar(50) NOT NULL DEFAULT '',
  PRIMARY KEY (`idx`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserFriends`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserFriends` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` tinyint(4) NOT NULL DEFAULT 0,
  `friendID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `friendPos` tinyint(4) NOT NULL DEFAULT 0,
  `friendName` varchar(50) NOT NULL DEFAULT '',
  PRIMARY KEY (`idx`),
  KEY `ownerID` (`ownerID`,`ownerPos`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserGarbage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserGarbage` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `itemNum` int(11) NOT NULL DEFAULT 0,
  `life` int(11) NOT NULL DEFAULT 0,
  `exValue1` int(11) NOT NULL DEFAULT 0,
  `exValue2` int(11) NOT NULL DEFAULT 0,
  `exValue3` int(11) NOT NULL DEFAULT 0,
  `serial` varchar(25) NOT NULL DEFAULT '',
  PRIMARY KEY (`idx`),
  KEY `ownerID` (`ownerID`),
  KEY `exValue` (`exValue1`,`exValue2`,`exValue3`),
  KEY `itemNum` (`itemNum`),
  KEY `serial` (`serial`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserGarbageMoney`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserGarbageMoney` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(100) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `amount` int(20) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `id` (`ownerID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserQuest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserQuest` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` int(11) NOT NULL DEFAULT 0,
  `ownerPos` int(11) NOT NULL DEFAULT 0,
  `questNum` int(11) NOT NULL DEFAULT 0,
  `questState` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `ownerIngo` (`ownerID`,`ownerPos`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserSkills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserSkills` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` int(11) NOT NULL DEFAULT 0,
  `skillNum` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `owner` (`ownerID`,`ownerPos`),
  KEY `skillNum` (`skillNum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserTable`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserTable` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `id` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `name` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `money` bigint(11) unsigned NOT NULL DEFAULT 0,
  `sex` tinyint(1) NOT NULL DEFAULT 0,
  `mapNum` int(11) NOT NULL DEFAULT 0,
  `posX` int(11) NOT NULL DEFAULT 0,
  `posY` int(11) NOT NULL DEFAULT 0,
  `hp` int(11) NOT NULL DEFAULT 0,
  `mp` int(11) NOT NULL DEFAULT 0,
  `str_point` int(11) NOT NULL DEFAULT 0,
  `int_point` int(11) NOT NULL DEFAULT 0,
  `dex_point` int(11) NOT NULL DEFAULT 0,
  `skill_exp0` int(11) NOT NULL DEFAULT 0,
  `skill_exp1` int(11) NOT NULL DEFAULT 0,
  `skill_exp2` int(11) NOT NULL DEFAULT 0,
  `skill_exp3` int(11) NOT NULL DEFAULT 0,
  `skill_level0` int(11) NOT NULL DEFAULT 0,
  `skill_level1` int(11) NOT NULL DEFAULT 0,
  `skill_level2` int(11) NOT NULL DEFAULT 0,
  `skill_level3` int(11) NOT NULL DEFAULT 0,
  `last_login` int(11) NOT NULL DEFAULT 0,
  `last_logout` int(11) NOT NULL DEFAULT 0,
  `charPos` int(4) NOT NULL DEFAULT 0,
  `headStyle` int(6) NOT NULL DEFAULT 0,
  `faceStyle` int(6) NOT NULL DEFAULT 0,
  `equipHead` int(11) NOT NULL DEFAULT 0,
  `equipJacket` int(11) NOT NULL DEFAULT 0,
  `equipPants` int(11) NOT NULL DEFAULT 0,
  `equipFoot` int(11) NOT NULL DEFAULT 0,
  `equipHand` int(11) NOT NULL DEFAULT 0,
  `str_exp` int(11) NOT NULL DEFAULT 0,
  `intelli_exp` int(11) NOT NULL DEFAULT 0,
  `dex_exp` int(11) NOT NULL DEFAULT 0,
  `max_hp` int(11) NOT NULL DEFAULT 0,
  `max_mp` int(11) NOT NULL DEFAULT 0,
  `mercType` int(11) NOT NULL DEFAULT 0,
  `mercLevel` int(11) NOT NULL DEFAULT 0,
  `mercExp` int(11) NOT NULL DEFAULT 0,
  `posLayer` int(11) NOT NULL DEFAULT 0,
  `cha` int(11) NOT NULL DEFAULT 0,
  `mapKind` int(11) NOT NULL DEFAULT 0,
  `str_add` int(11) NOT NULL DEFAULT 0,
  `int_add` int(11) NOT NULL DEFAULT 0,
  `dex_add` int(11) NOT NULL DEFAULT 0,
  `guildNo` int(11) NOT NULL DEFAULT 0,
  `guildLevel` int(11) NOT NULL DEFAULT 0,
  `prevMapNum` int(11) NOT NULL DEFAULT 0,
  `prevPosX` int(11) NOT NULL DEFAULT 0,
  `prevPosY` int(11) NOT NULL DEFAULT 0,
  `lastSkill` int(11) NOT NULL DEFAULT 0,
  `isNewChar` tinyint(4) NOT NULL DEFAULT 0,
  `charClass` tinyint(4) NOT NULL DEFAULT 0,
  `tempGuild` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `id` (`id`),
  KEY `name` (`name`),
  KEY `charPos` (`charPos`),
  KEY `guildNo` (`guildNo`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `WarData`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `WarData` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerGuild` int(11) NOT NULL DEFAULT 0,
  `ownerPoint` int(11) NOT NULL DEFAULT 0,
  `enemyGuild` int(11) NOT NULL DEFAULT 0,
  `targetMapNum` int(11) NOT NULL DEFAULT 0,
  `type` smallint(6) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `owner` (`ownerGuild`),
  KEY `enemy` (`enemyGuild`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `LAQIA_ITEMLOG` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */;

USE `LAQIA_ITEMLOG`;
DROP TABLE IF EXISTS `BillItemLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `BillItemLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` tinyint(4) NOT NULL DEFAULT 0,
  `itemNum` int(11) NOT NULL DEFAULT 0,
  `type` tinyint(4) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `owner` (`ownerID`,`ownerPos`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `CashMoneyLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `CashMoneyLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `amount` int(11) NOT NULL DEFAULT 0,
  `action` smallint(11) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `owerID` (`ownerID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `GarbageLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `GarbageLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` tinyint(4) NOT NULL DEFAULT 0,
  `itemNum` int(11) NOT NULL DEFAULT 0,
  `action` tinyint(4) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  `serial` varchar(25) NOT NULL DEFAULT '',
  PRIMARY KEY (`idx`),
  KEY `ownerID` (`ownerID`,`ownerPos`),
  KEY `serial` (`serial`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `GarbageMoneyLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `GarbageMoneyLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `amount` int(11) NOT NULL DEFAULT 0,
  `action` smallint(11) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `owerID` (`ownerID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `ItemLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `ItemLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` tinyint(4) NOT NULL DEFAULT 0,
  `itemNum` int(11) NOT NULL DEFAULT 0,
  `action` tinyint(4) NOT NULL DEFAULT 0,
  `actionID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `actionPos` tinyint(4) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  `serial` varchar(25) NOT NULL DEFAULT '',
  PRIMARY KEY (`idx`),
  KEY `actionID` (`actionID`,`actionPos`),
  KEY `ownerID` (`ownerID`,`ownerPos`),
  KEY `serial` (`serial`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `LoginLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `LoginLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `id` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `name` varchar(50) NOT NULL DEFAULT '',
  `ip` varchar(50) NOT NULL DEFAULT '',
  `date` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `name` (`name`),
  KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `MoneyLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `MoneyLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` tinyint(4) NOT NULL DEFAULT 0,
  `amount` int(11) NOT NULL DEFAULT 0,
  `action` tinyint(4) NOT NULL DEFAULT 0,
  `actionID` varchar(50) NOT NULL DEFAULT '',
  `actionPos` tinyint(4) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`),
  KEY `actionID` (`actionID`,`actionPos`),
  KEY `ownerID` (`ownerID`,`ownerPos`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `MyShopLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `MyShopLog` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `ownerID` varchar(50) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ownerPos` tinyint(4) NOT NULL DEFAULT 0,
  `actionID` varchar(50) NOT NULL DEFAULT '',
  `actionPos` tinyint(4) NOT NULL DEFAULT 0,
  `itemNum` int(11) NOT NULL DEFAULT 0,
  `price` int(11) NOT NULL DEFAULT 0,
  `date` int(11) NOT NULL DEFAULT 0,
  `serial` varchar(25) NOT NULL DEFAULT '',
  PRIMARY KEY (`idx`),
  KEY `actionID` (`actionID`,`actionPos`),
  KEY `serial` (`serial`),
  KEY `ownerID` (`ownerID`,`ownerPos`),
  KEY `itemNum` (`itemNum`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `LAQIA_CASH` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */;

USE `LAQIA_CASH`;
DROP TABLE IF EXISTS `StorCash`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `StorCash` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `UserID` varchar(100) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `Status` int(11) NOT NULL DEFAULT 0,
  `Cash` int(11) unsigned NOT NULL DEFAULT 0,
  `Invoice` int(11) NOT NULL DEFAULT 0,
  `Date` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`idx`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `UserCashMoney`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `UserCashMoney` (
  `idx` bigint(20) NOT NULL AUTO_INCREMENT,
  `UserID` varchar(100) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `amount` int(11) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`idx`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `DAUM_BILLING` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci */;

USE `DAUM_BILLING`;
DROP TABLE IF EXISTS `PurSell`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `PurSell` (
  `PurKey` varchar(21) NOT NULL DEFAULT '',
  `UserUID` varchar(14) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `ProductNum` int(11) NOT NULL DEFAULT 0,
  `ProductPrice` int(11) NOT NULL DEFAULT 0,
  `PurFlag` int(11) NOT NULL DEFAULT 0,
  `PurDate` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `PurChgDate` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`PurKey`),
  KEY `UserUID` (`UserUID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `REWARD`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `REWARD` (
  `UserUID` varchar(100) NOT NULL DEFAULT '',
  `PurFlag` tinyint(1) NOT NULL DEFAULT 0
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
DROP TABLE IF EXISTS `sd`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `sd` (
  `sd` varchar(100) NOT NULL DEFAULT ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

