-- Hotfix cache: the client fetches the timewalking items without resilience again
UPDATE `version` SET `hotfix_cache_id` = `hotfix_cache_id` + 1;
