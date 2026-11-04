-- Make the clients reload the hotfix records whose Russian texts were replaced
UPDATE `version` SET `hotfix_cache_id` = `hotfix_cache_id` + 1;
