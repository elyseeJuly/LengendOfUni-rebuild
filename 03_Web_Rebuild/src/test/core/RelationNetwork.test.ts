import { describe, it, expect, beforeEach } from 'vitest';
import { RelationNetwork } from '../../core/RelationNetwork';

describe('RelationNetwork', () => {
  let rn: RelationNetwork;

  beforeEach(() => {
    rn = new RelationNetwork();
  });

  it('初始 relations 为空', () => {
    expect(rn.relations).toEqual([]);
  });

  it('establishRelation 创建关系', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 85);
    expect(rn.relations.length).toBe(1);
    expect(rn.relations[0].personA).toBe('罗辑');
    expect(rn.relations[0].personB).toBe('史强');
    expect(rn.relations[0].relationType).toBe('ALLY');
    expect(rn.relations[0].intensity).toBe(85);
  });

  it('establishRelation 重复关系替换旧关系', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 85);
    rn.establishRelation('罗辑', '史强', 'ALLY', 15, 'canon', 90);
    expect(rn.relations.length).toBe(1);
    expect(rn.relations[0].intensity).toBe(90);
    expect(rn.relations[0].establishedYear).toBe(15);
  });

  it('establishRelation 相同两人不同关系并存', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 85);
    rn.establishRelation('罗辑', '史强', 'RIVAL', 15, 'event', 60);
    expect(rn.relations.length).toBe(2);
  });

  it('modifyRelationByEvent 改变强度', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 50);
    rn.modifyRelationByEvent('罗辑', '史强', 20);
    expect(rn.relations[0].intensity).toBe(70);
    rn.modifyRelationByEvent('罗辑', '史强', -40);
    expect(rn.relations[0].intensity).toBe(30);
  });

  it('modifyRelationByEvent 强度限制在0-100', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 90);
    rn.modifyRelationByEvent('罗辑', '史强', 50);
    expect(rn.relations[0].intensity).toBe(100);
    rn.modifyRelationByEvent('罗辑', '史强', -200);
    expect(rn.relations[0].intensity).toBe(0);
  });

  it('getRelation 双向查找', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 85);
    const rel1 = rn.getRelation('罗辑', '史强');
    expect(rel1).not.toBeNull();
    expect(rel1!.relationType).toBe('ALLY');
    const rel2 = rn.getRelation('史强', '罗辑');
    expect(rel2).not.toBeNull();
    expect(rel2!.relationType).toBe('ALLY');
  });

  it('getRelation 不存在返回 null', () => {
    expect(rn.getRelation('不存在', '罗辑')).toBeNull();
  });

  it('getPersonRelations 返回角色所有关系', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 85);
    rn.establishRelation('罗辑', '庄颜', 'ALLY', 10, 'canon', 90);
    rn.establishRelation('程心', '云天明', 'ALLY', 20, 'canon', 85);
    const luoRelations = rn.getPersonRelations('罗辑');
    expect(luoRelations.length).toBe(2);
    const chengRelations = rn.getPersonRelations('程心');
    expect(chengRelations.length).toBe(1);
  });

  it('getPersonRelations 不存在的角色返回空数组', () => {
    expect(rn.getPersonRelations('不存在')).toEqual([]);
  });

  it('getRelationScore 计算平均强度', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 80);
    rn.establishRelation('罗辑', '庄颜', 'ALLY', 10, 'canon', 90);
    const score = rn.getRelationScore({ personA: '罗辑' });
    expect(score).toBe(85);
  });

  it('getRelationScore 按类型筛选', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 80);
    rn.establishRelation('伊文斯', '罗辑', 'RIVAL', 10, 'canon', 95);
    const allyScore = rn.getRelationScore({ personA: '罗辑', type: 'ALLY' });
    expect(allyScore).toBe(80);
    const rivalScore = rn.getRelationScore({ personA: '罗辑', type: 'RIVAL' });
    expect(rivalScore).toBe(95);
  });

  it('getRelationScore 无匹配时返回 0', () => {
    expect(rn.getRelationScore({ personA: '不存在' })).toBe(0);
  });

  it('getRelationCount 按类型统计', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 85);
    rn.establishRelation('程心', '艾AA', 'ALLY', 20, 'canon', 80);
    rn.establishRelation('伊文斯', '罗辑', 'RIVAL', 10, 'canon', 95);
    expect(rn.getRelationCount('ALLY')).toBe(2);
    expect(rn.getRelationCount('RIVAL')).toBe(1);
    expect(rn.getRelationCount('NEUTRAL')).toBe(0);
  });

  it('initCanonicalRelations 创建预设关系', () => {
    rn.initCanonicalRelations(0);
    expect(rn.relations.length).toBeGreaterThan(0);
    expect(rn.getRelation('泰勒', '破壁人1号')).not.toBeNull();
    expect(rn.getRelation('罗辑', '史强')).not.toBeNull();
    expect(rn.getRelation('程心', '云天明')).not.toBeNull();
  });

  it('initCanonicalRelations 包含所有经典关系', () => {
    rn.initCanonicalRelations(0);
    const expectedPairs = [
      ['泰勒', '破壁人1号'],
      ['雷迪亚兹', '破壁人2号'],
      ['希恩斯', '破壁人3号'],
      ['罗辑', '破壁人'],
      ['罗辑', '史强'],
      ['罗辑', '庄颜'],
      ['程心', '云天明'],
      ['程心', '艾AA'],
      ['章北海', '东方延续'],
      ['伊文斯', '罗辑'],
      ['伊文斯', '史强'],
    ];
    for (const [a, b] of expectedPairs) {
      expect(rn.getRelation(a, b)).not.toBeNull();
    }
  });

  it('toJSON/fromJSON 序列化往返', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 85);
    rn.establishRelation('伊文斯', '罗辑', 'RIVAL', 10, 'canon', 95);
    const json = rn.toJSON();
    const restored = RelationNetwork.fromJSON(json);
    expect(restored.relations.length).toBe(2);
    expect(restored.getRelation('罗辑', '史强')!.intensity).toBe(85);
    expect(restored.getRelation('伊文斯', '罗辑')!.relationType).toBe('RIVAL');
  });

  it('fromJSON 空数据不报错', () => {
    const rn2 = RelationNetwork.fromJSON(null);
    expect(rn2.relations).toEqual([]);
    const rn3 = RelationNetwork.fromJSON({});
    expect(rn3.relations).toEqual([]);
  });

  it('reset 清除所有关系', () => {
    rn.establishRelation('罗辑', '史强', 'ALLY', 10, 'canon', 85);
    rn.reset();
    expect(rn.relations).toEqual([]);
  });

  it('establishRelation 默认强度为50', () => {
    rn.establishRelation('罗辑', '庄颜', 'ALLY', 10, 'canon');
    expect(rn.relations[0].intensity).toBe(50);
  });
});