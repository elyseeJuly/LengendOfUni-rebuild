import { describe, it, expect, beforeEach } from 'vitest';
import { TagManager, STANDARD_TAGS } from '../../core/TagManager';

describe('TagManager', () => {
  let tm: TagManager;

  beforeEach(() => {
    tm = new TagManager();
  });

  it('applyWorldTag 创建新标签', () => {
    tm.applyWorldTag('tech_boom', 50, 'test_event', 10);
    expect(tm.hasTag('tech_boom')).toBe(true);
    expect(tm.getTagIntensity('tech_boom')).toBe(50);
  });

  it('applyWorldTag 重复标签合并强度', () => {
    tm.applyWorldTag('tech_boom', 40, 'test_event', 10);
    tm.applyWorldTag('tech_boom', 30, 'test_event2', 11);
    expect(tm.getTagIntensity('tech_boom')).toBe(70);
  });

  it('applyWorldTag 强度不超过100', () => {
    tm.applyWorldTag('tech_boom', 80, 'test_event', 10);
    tm.applyWorldTag('tech_boom', 50, 'test_event2', 11);
    expect(tm.getTagIntensity('tech_boom')).toBe(100);
  });

  it('applyWorldTag 未知标签发出警告不抛出', () => {
    const consoleWarn = vi.spyOn(console, 'warn').mockImplementation(() => {});
    tm.applyWorldTag('unknown_tag', 50, 'test', 10);
    expect(consoleWarn).toHaveBeenCalled();
    expect(tm.hasTag('unknown_tag')).toBe(false);
    consoleWarn.mockRestore();
  });

  it('setWorldTagIntensity 设置精确值', () => {
    tm.setWorldTagIntensity('civil_unrest', 75, 10, 'manual');
    expect(tm.getTagIntensity('civil_unrest')).toBe(75);
  });

  it('setWorldTagIntensity 限制在0-100范围', () => {
    tm.setWorldTagIntensity('civil_unrest', 150, 10, 'manual');
    expect(tm.getTagIntensity('civil_unrest')).toBe(100);
    tm.setWorldTagIntensity('civil_unrest', -10, 11, 'manual');
    expect(tm.getTagIntensity('civil_unrest')).toBe(0);
  });

  it('decayTags 减少非里程碑标签强度', () => {
    tm.applyWorldTag('civil_unrest', 80, 'test', 10);
    tm.decayTags(12);
    expect(tm.getTagIntensity('civil_unrest')).toBe(74);
  });

  it('decayTags 强度降至0时移除标签', () => {
    tm.applyWorldTag('civil_unrest', 5, 'test', 10);
    tm.decayTags(12);
    expect(tm.hasTag('civil_unrest')).toBe(false);
  });

  it('decayTags 不衰减里程碑标签', () => {
    tm.applyWorldTag('space_force_built', 100, 'test', 10);
    tm.decayTags(30);
    expect(tm.getTagIntensity('space_force_built')).toBe(100);
  });

  it('hasTag 返回正确结果', () => {
    expect(tm.hasTag('tech_boom')).toBe(false);
    tm.applyWorldTag('tech_boom', 30, 'test', 10);
    expect(tm.hasTag('tech_boom')).toBe(true);
    expect(tm.hasTag('tech_boom', 50)).toBe(false);
  });

  it('getTagIntensity 返回正确强度', () => {
    expect(tm.getTagIntensity('nonexistent')).toBe(0);
    tm.applyWorldTag('resource_depleted', 60, 'test', 10);
    expect(tm.getTagIntensity('resource_depleted')).toBe(60);
  });

  it('removeWorldTag 移除标签', () => {
    tm.applyWorldTag('tech_boom', 50, 'test', 10);
    expect(tm.hasTag('tech_boom')).toBe(true);
    tm.removeWorldTag('tech_boom');
    expect(tm.hasTag('tech_boom')).toBe(false);
  });

  it('removeWorldTag 移除不存在的标签不报错', () => {
    expect(() => tm.removeWorldTag('nonexistent')).not.toThrow();
  });

  it('applyCharacterTag 创建角色标签', () => {
    tm.applyCharacterTag('罗辑', {
      personName: '罗辑', tagId: 'pro_humanity', tagName: '为人类做贡献',
      value: 70, appliedYear: 10, source: 'test',
    });
    const tags = tm.characterTags.get('罗辑')!;
    expect(tags.length).toBe(1);
    expect(tags[0].tagId).toBe('pro_humanity');
  });

  it('applyCharacterTag 重复标签合并', () => {
    const tag = { personName: '罗辑', tagId: 'pro_humanity', tagName: '为人类做贡献',
      value: 50, appliedYear: 10, source: 'test' };
    tm.applyCharacterTag('罗辑', tag);
    tm.applyCharacterTag('罗辑', { ...tag, value: 30, appliedYear: 11 });
    const tags = tm.characterTags.get('罗辑')!;
    expect(tags.length).toBe(1);
    expect(tags[0].value).toBe(80);
  });

  it('getCharacterStance 返回最高值标签', () => {
    tm.applyCharacterTag('程心', {
      personName: '程心', tagId: 'detached', tagName: '脱离社会',
      value: 30, appliedYear: 10, source: 'test',
    });
    tm.applyCharacterTag('程心', {
      personName: '程心', tagId: 'betrayer', tagName: '人类叛徒',
      value: 60, appliedYear: 11, source: 'test',
    });
    expect(tm.getCharacterStance('程心')).toBe('betrayer');
  });

  it('getCharacterStance 无标签时返回null', () => {
    expect(tm.getCharacterStance('不存在')).toBeNull();
  });

  it('toJSON/fromJSON 序列化往返', () => {
    tm.applyWorldTag('tech_boom', 60, 'test', 10);
    tm.applyWorldTag('civil_unrest', 40, 'test', 10);
    tm.applyCharacterTag('罗辑', {
      personName: '罗辑', tagId: 'pro_humanity', tagName: '为人类做贡献',
      value: 85, appliedYear: 10, source: 'canon',
    });
    tm.applyRegionTag('地下城A区', {
      id: 'region_test', name: '测试区域', intensity: 50,
      firstAppliedYear: 10, lastReinforcedYear: 10,
      source: 'test', isMilestone: false, category: 'social',
    });
    tm.applyOrgTag('ETO', {
      id: 'org_test', name: '测试组织', intensity: 70,
      firstAppliedYear: 10, lastReinforcedYear: 10,
      source: 'test', isMilestone: false, category: 'social',
    });

    const json = tm.toJSON();
    const restored = TagManager.fromJSON(json);

    expect(restored.getTagIntensity('tech_boom')).toBe(tm.getTagIntensity('tech_boom'));
    expect(restored.getTagIntensity('civil_unrest')).toBe(tm.getTagIntensity('civil_unrest'));
    expect(restored.hasTag('tech_boom')).toBe(true);

    const charTags = restored.characterTags.get('罗辑')!;
    expect(charTags.length).toBe(1);
    expect(charTags[0].tagId).toBe('pro_humanity');

    expect(restored.regionTags.size).toBe(1);
    expect(restored.orgTags.size).toBe(1);
  });

  it('fromJSON 空数据不报错', () => {
    const tm2 = TagManager.fromJSON(null);
    expect(tm2.worldTags.size).toBe(0);
    const tm3 = TagManager.fromJSON({});
    expect(tm3.worldTags.size).toBe(0);
  });

  it('reset 清除所有标签', () => {
    tm.applyWorldTag('tech_boom', 60, 'test', 10);
    tm.applyCharacterTag('罗辑', {
      personName: '罗辑', tagId: 'pro_humanity', tagName: '为人类做贡献',
      value: 85, appliedYear: 10, source: 'canon',
    });
    tm.reset();
    expect(tm.worldTags.size).toBe(0);
    expect(tm.characterTags.size).toBe(0);
    expect(tm.regionTags.size).toBe(0);
    expect(tm.orgTags.size).toBe(0);
  });

  it('STANDARD_TAGS 包含所有必要条目', () => {
    const requiredTags = [
      'resource_depleted', 'resource_surplus', 'tech_boom', 'population_crisis',
      'civil_unrest', 'eto_remnant', 'new_apostle', 'exile_sentiment',
      'social_split', 'underground_gangs', 'digital_religion',
      'space_force_built', 'deterrence_steady', 'deterrence_unstable',
      'waterdrop_used', 'foil_imminent', 'mil_threat',
      'crisis_era_deep', 'deterrence_era', 'broadcast_era', 'bunker_era_deep', 'galaxy_era_deep',
    ];
    for (const tagId of requiredTags) {
      expect(STANDARD_TAGS[tagId]).toBeDefined();
      expect(typeof STANDARD_TAGS[tagId].name).toBe('string');
      expect(typeof STANDARD_TAGS[tagId].category).toBe('string');
      expect(typeof STANDARD_TAGS[tagId].isMilestone).toBe('boolean');
    }
  });

  it('里程碑标签不衰减', () => {
    const milestoneTags = Object.entries(STANDARD_TAGS)
      .filter(([_, def]) => def.isMilestone)
      .map(([id]) => id);
    expect(milestoneTags.length).toBeGreaterThan(0);
    for (const id of milestoneTags) {
      tm.applyWorldTag(id, 100, 'test', 10);
    }
    tm.decayTags(50);
    for (const id of milestoneTags) {
      expect(tm.getTagIntensity(id)).toBe(100);
    }
  });

  it('getTagsByCategory 按分类筛选', () => {
    tm.applyWorldTag('tech_boom', 50, 'test', 10);
    tm.applyWorldTag('civil_unrest', 60, 'test', 10);
    tm.applyWorldTag('space_force_built', 100, 'test', 10);
    tm.applyWorldTag('crisis_era_deep', 100, 'test', 10);

    expect(tm.getTagsByCategory('state').length).toBe(1);
    expect(tm.getTagsByCategory('social').length).toBe(1);
    expect(tm.getTagsByCategory('military').length).toBe(1);
    expect(tm.getTagsByCategory('epoch').length).toBe(1);
  });

  it('getCharacterTags 获取角色所有标签', () => {
    expect(tm.getCharacterTags('不存在')).toEqual([]);
    tm.applyCharacterTag('罗辑', {
      personName: '罗辑', tagId: 'pro_humanity', tagName: '为人类做贡献',
      value: 85, appliedYear: 10, source: 'canon',
    });
    const tags = tm.getCharacterTags('罗辑');
    expect(tags.length).toBe(1);
  });
});