CREATE TABLE `module_system_git` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `account_id` int(10) NOT NULL,
  `category_id` int(10) DEFAULT NULL,
  `url` varchar(255) NOT NULL,
  `clone_url` varchar(255) NOT NULL,
  `title` varchar(255) NOT NULL UNIQUE,
  `abstract` text,
  `type` varchar (255) NOT NULL,
  `published` boolean DEFAULT '0',
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;


CREATE TABLE `module_system_git_type` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;

INSERT INTO `module_system_git_type` VALUES (1, 'Module');
INSERT INTO `module_system_git_type` VALUES (2, 'Template');
INSERT INTO `module_system_git_type` VALUES (3, 'Profile');