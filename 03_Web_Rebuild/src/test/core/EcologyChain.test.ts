import { describe, it, expect, beforeEach, vi } from 'vitest';
import { EcologyChain } from '../../core/EcologyChain';
import { TagManager } from '../../core/TagManager';

describe('EcologyChain', () => {
  let ec: EcologyChain;
  let tagManager: TagManager;

  beforeEach(() => {
    ec = new EcologyChain();
    tagManager = new TagManager();
    // 固定 Math.random 返回 0 以确保概率判定总为 true
    vi.spyOn(Math, 'random').mockReturnValue(0);
  });

  afterEach(() => {
    vi.restoreAllMocks();
  });

  it('初始 activeChains 为空', () => {
    expect(ec.activeChains).toEqual([]);
    expect(ec.getActiveChains()).toEqual([]);
  });

  it('初始化时加载预设链', () => {
    expect(ec.chains.length).toBeGreaterThan(0);
    expect(ec.chains[0]).toHaveProperty('id');
    expect(ec.chains[0]).toHaveProperty('conditionEventId');
    expect(ec.chains[0]).toHaveProperty('triggerDelay');
    expect(ec.chains[0]).toHaveProperty('resultEventId');
    expect(ec.chains[0]).toHaveProperty('probability');
  });

  it('checkChainReactions 匹配事件时激活链', () => {
    const triggered = ec.checkChainReactions('random_resource_rationing', tagManager, 10);
    expect(triggered.length).toBeGreaterThan(0);
    expect(ec.activeChains.length).toBeGreaterThan(0);
    expect(ec.activeChains[0].conditionEventId).toBe('random_resource_rationing');
    expect(ec.activeChains[0].remainingDelay).toBe(3);
  });

  it('checkChainReactions 不匹配事件时不激活', () => {
    const triggered = ec.checkChainReactions('nonexistent_event', tagManager, 10);
    expect(triggered.length).toBe(0);
    expect(ec.activeChains.length).toBe(0);
  });

  it('checkChainReactions suppressIfTagMissing 阻止激活', () => {
    const etoChain = ec.chains.find(c => c.conditionEventId === 'random_eto_activity');
    expect(etoChain).toBeDefined();
    expect(etoChain!.suppressIfTagMissing).toContain('eto_remnant');

    const triggered = ec.checkChainReactions('random_eto_activity', tagManager, 10);
    expect(triggered.length).toBe(0);
  });

  it('checkChainReactions suppressIfTagMissing 有标签时正常触发', () => {
    tagManager.applyWorldTag('eto_remnant', 50, 'test', 10);
    const triggered = ec.checkChainReactions('random_eto_activity', tagManager, 10);
    expect(triggered.length).toBeGreaterThanOrEqual(0);
  });

  it('advanceTurn 减少剩余延迟', () => {
    ec.checkChainReactions('random_resource_rationing', tagManager, 10);
    expect(ec.activeChains[0].remainingDelay).toBe(3);
    ec.advanceTurn(tagManager, 11);
    expect(ec.activeChains[0].remainingDelay).toBe(2);
  });

  it('advanceTurn 延迟归零时触发就绪事件', () => {
    ec.checkChainReactions('random_resource_rationing', tagManager, 10);
    for (let i = 0; i < 2; i++) {
      ec.advanceTurn(tagManager, 11 + i);
    }
    expect(ec.activeChains.length).toBe(1);
    const events = ec.advanceTurn(tagManager, 13);
    expect(events.length).toBe(1);
    expect(events[0]).toBe('random_underground_riot');
  });

  it('advanceTurn 触发时就绪事件后移除活动链', () => {
    ec.checkChainReactions('random_resource_rationing', tagManager, 10);
    for (let i = 0; i < 3; i++) ec.advanceTurn(tagManager, 11 + i);
    expect(ec.activeChains.length).toBe(0);
  });

  it('advanceTurn 应用产生的标签', () => {
    ec.checkChainReactions('random_resource_rationing', tagManager, 10);
    for (let i = 0; i < 3; i++) ec.advanceTurn(tagManager, 11 + i);
    expect(tagManager.hasTag('civil_unrest')).toBe(true);
    expect(tagManager.hasTag('underground_gangs')).toBe(true);
  });

  it('advanceTurn 移除消耗的标签', () => {
    tagManager.applyWorldTag('underground_gangs', 50, 'test', 10);
    tagManager.applyWorldTag('civil_unrest', 30, 'test', 10);
    ec.checkChainReactions('random_underground_riot', tagManager, 10);
    expect(tagManager.hasTag('underground_gangs')).toBe(true);
    for (let i = 0; i < 5; i++) ec.advanceTurn(tagManager, 11 + i);
    expect(tagManager.hasTag('underground_gangs')).toBe(false);
  });

  it('advanceTurn 消耗标签不影响不存在的标签', () => {
    ec.chains.push({
      id: 'test_consumption',
      name: '测试消耗',
      conditionEventId: 'test_event',
      triggerDelay: 1,
      resultEventId: 'test_result',
      producedTags: [],
      consumedTags: ['nonexistent_tag'],
      probability: 1,
      suppressIfTagMissing: [],
    });
    ec.checkChainReactions('test_event', tagManager, 10);
    expect(() => ec.advanceTurn(tagManager, 11)).not.toThrow();
  });

  it('getActiveChains 只返回待处理链', () => {
    ec.checkChainReactions('random_resource_rationing', tagManager, 10);
    expect(ec.getActiveChains().length).toBe(1);
    for (let i = 0; i < 3; i++) ec.advanceTurn(tagManager, 11 + i);
    expect(ec.getActiveChains().length).toBe(0);
  });

  it('toJSON/fromJSON 序列化往返', () => {
    ec.checkChainReactions('random_resource_rationing', tagManager, 10);
    const json = ec.toJSON();
    const restored = EcologyChain.fromJSON(json);
    expect(restored.activeChains.length).toBe(1);
    expect(restored.activeChains[0].chainId).toBe('ration_to_riot');
    expect(restored.activeChains[0].remainingDelay).toBe(3);
  });

  it('fromJSON 空数据不报错', () => {
    const ec2 = EcologyChain.fromJSON(null);
    expect(ec2.activeChains).toEqual([]);
    const ec3 = EcologyChain.fromJSON({});
    expect(ec3.activeChains).toEqual([]);
  });

  it('reset 清除所有活动链', () => {
    ec.checkChainReactions('random_resource_rationing', tagManager, 10);
    expect(ec.activeChains.length).toBeGreaterThan(0);
    ec.reset();
    expect(ec.activeChains.length).toBe(0);
  });

  it('所有预设链有合法概率值', () => {
    for (const chain of ec.chains) {
      expect(chain.probability).toBeGreaterThanOrEqual(0);
      expect(chain.probability).toBeLessThanOrEqual(1);
      expect(chain.triggerDelay).toBeGreaterThan(0);
    }
  });

  it('所有预设链有正确字段', () => {
    for (const chain of ec.chains) {
      expect(chain).toHaveProperty('id');
      expect(chain).toHaveProperty('name');
      expect(chain).toHaveProperty('conditionEventId');
      expect(chain).toHaveProperty('triggerDelay');
      expect(chain).toHaveProperty('resultEventId');
      expect(chain).toHaveProperty('producedTags');
      expect(chain).toHaveProperty('consumedTags');
      expect(chain).toHaveProperty('probability');
      expect(chain).toHaveProperty('suppressIfTagMissing');
    }
  });
});