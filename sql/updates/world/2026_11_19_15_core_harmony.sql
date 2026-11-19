-- Spring Blossoms (Efflorescence): applied by the original caster so Mastery: Harmony and the druid's stats apply
UPDATE `spell_linked_spell` SET `caster` = 10 WHERE `spell_trigger` = 81269 AND `spell_effect` = 207386 AND `type` = 9;
