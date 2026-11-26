-- Hotfix cache: the client fetches the corrected holiday dates again
UPDATE `version` SET `hotfix_cache_id` = `hotfix_cache_id` + 1;
