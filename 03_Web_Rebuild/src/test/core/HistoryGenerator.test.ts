import { describe, it, expect, beforeEach } from 'vitest';
import { HistoryGenerator } from '../../core/HistoryGenerator';

describe('HistoryGenerator', () => {
  let hg: HistoryGenerator;

  beforeEach(() => {
    hg = new HistoryGenerator();
  });

  it('初始 entries 为空', () => {
    expect(hg.entries).toEqual([]);
  });

  it('recordMilestone 添加 MILESTONE 条目', () => {
    hg.recordMilestone(10, 0, '面壁计划启动', '联合国正式启动面壁计划');
    expect(hg.entries.length).toBe(1);
    expect(hg.entries[0].entryType).toBe('MILESTONE');
    expect(hg.entries[0].year).toBe(10);
    expect(hg.entries[0].epochName).toBe('危机纪元');
  });

  it('recordEvent 添加 EVENT 条目', () => {
    hg.recordEvent(20, 1, '三体探测器抵达', '水滴进入太阳系');
    expect(hg.entries.length).toBe(1);
    expect(hg.entries[0].entryType).toBe('EVENT');
    expect(hg.entries[0].epochName).toBe('威慑纪元');
  });

  it('recordTagChange 添加 TAG_APPLIED 条目', () => {
    hg.recordTagChange(15, 0, 'tech_boom', '科技爆发期', true);
    expect(hg.entries.length).toBe(1);
    expect(hg.entries[0].entryType).toBe('TAG_APPLIED');
    expect(hg.entries[0].title).toContain('科技爆发期');
  });

  it('recordTagChange 添加 TAG_REMOVED 条目', () => {
    hg.recordTagChange(25, 0, 'tech_boom', '科技爆发期', false);
    expect(hg.entries[0].entryType).toBe('TAG_REMOVED');
    expect(hg.entries[0].title).toContain('消失');
  });

  it('recordCrisis 添加 CRISIS 条目', () => {
    hg.recordCrisis(30, 2, '二向箔打击', '太阳系二维化开始');
    expect(hg.entries.length).toBe(1);
    expect(hg.entries[0].entryType).toBe('CRISIS');
    expect(hg.entries[0].epochName).toBe('广播纪元');
  });

  it('recordVictory 添加 VICTORY 条目', () => {
    hg.recordVictory(400, 4, '死神永生', '人类归还小宇宙质量');
    expect(hg.entries.length).toBe(1);
    expect(hg.entries[0].entryType).toBe('VICTORY');
    expect(hg.entries[0].epochName).toBe('银河纪元');
  });

  it('generateChronicle 返回格式化文本', () => {
    hg.recordMilestone(10, 0, '面壁计划启动', '联合国正式启动面壁计划');
    hg.recordEvent(20, 1, '水滴抵达', '三体探测器进入太阳系', ['deterrence_unstable'], ['罗辑']);
    const chronicle = hg.generateChronicle();
    expect(chronicle).toContain('[危机纪元 第10年]');
    expect(chronicle).toContain('面壁计划启动');
    expect(chronicle).toContain('[威慑纪元 第20年]');
    expect(chronicle).toContain('水滴抵达');
    expect(chronicle).toContain('deterrence_unstable');
    expect(chronicle).toContain('罗辑');
  });

  it('generateChronicle 空记录时返回提示', () => {
    expect(hg.generateChronicle()).toBe('暂无历史记录。');
  });

  it('getEntriesByEpoch 按纪元筛选', () => {
    hg.recordMilestone(10, 0, '事件1', '描述1');
    hg.recordEvent(20, 1, '事件2', '描述2');
    hg.recordEvent(30, 0, '事件3', '描述3');
    const crisisEntries = hg.getEntriesByEpoch(0);
    expect(crisisEntries.length).toBe(2);
    const deterrenceEntries = hg.getEntriesByEpoch(1);
    expect(deterrenceEntries.length).toBe(1);
  });

  it('getEntriesByEpoch 不存在的纪元返回空数组', () => {
    const entries = hg.getEntriesByEpoch(99);
    expect(entries).toEqual([]);
  });

  it('getRecentEntries 返回最新记录倒序', () => {
    hg.recordMilestone(10, 0, '事件1', '描述');
    hg.recordEvent(20, 1, '事件2', '描述');
    hg.recordCrisis(30, 2, '事件3', '描述');
    const recent = hg.getRecentEntries(2);
    expect(recent.length).toBe(2);
    expect(recent[0].title).toBe('事件3');
    expect(recent[1].title).toBe('事件2');
  });

  it('getRecentEntries 数量不足时返回全部', () => {
    hg.recordMilestone(10, 0, '事件1', '描述');
    const recent = hg.getRecentEntries(10);
    expect(recent.length).toBe(1);
  });

  it('exportToTimeline 返回数组', () => {
    hg.recordMilestone(10, 0, '面壁计划', '启动');
    hg.recordEvent(20, 1, '水滴抵达', '进入太阳系');
    const timeline = hg.exportToTimeline();
    expect(timeline.length).toBe(2);
    expect(timeline[0]).toHaveProperty('year');
    expect(timeline[0]).toHaveProperty('epoch');
    expect(timeline[0]).toHaveProperty('title');
    expect(timeline[0]).toHaveProperty('description');
    expect(timeline[0]).toHaveProperty('type');
  });

  it('prune 删除超出限制的旧记录', () => {
    for (let i = 0; i < 20; i++) {
      hg.recordEvent(i, 0, `事件${i}`, `描述${i}`);
    }
    expect(hg.entries.length).toBe(20);
    hg.prune(10);
    expect(hg.entries.length).toBe(10);
    expect(hg.entries[0].title).toBe('事件10');
  });

  it('prune 不超过限制时不删除', () => {
    hg.recordEvent(10, 0, '事件', '描述');
    hg.prune(500);
    expect(hg.entries.length).toBe(1);
  });

  it('incTurn 增加计数', () => {
    expect((hg as any).turnCounter).toBe(0);
    hg.incTurn();
    expect((hg as any).turnCounter).toBe(1);
    hg.incTurn();
    expect((hg as any).turnCounter).toBe(2);
  });

  it('toJSON/fromJSON 序列化往返', () => {
    hg.recordMilestone(10, 0, '面壁计划', '启动');
    hg.recordEvent(20, 1, '水滴抵达', '进入太阳系', ['deterrence_unstable']);
    hg.incTurn();
    hg.incTurn();
    const json = hg.toJSON();
    const restored = HistoryGenerator.fromJSON(json);
    expect(restored.entries.length).toBe(2);
    expect(restored.entries[0].title).toBe('面壁计划');
    expect(restored.entries[1].relatedTags).toContain('deterrence_unstable');
    expect((restored as any).turnCounter).toBe(2);
  });

  it('fromJSON 空数据不报错', () => {
    const hg2 = HistoryGenerator.fromJSON(null);
    expect(hg2.entries).toEqual([]);
    expect((hg2 as any).turnCounter).toBe(0);
  });

  it('reset 清除所有条目和计数', () => {
    hg.recordMilestone(10, 0, '事件', '描述');
    hg.incTurn();
    hg.reset();
    expect(hg.entries).toEqual([]);
    expect((hg as any).turnCounter).toBe(0);
  });

  it('记录条目包含相关人员和标签', () => {
    hg.recordMilestone(10, 0, '面壁计划', '启动', ['crisis_era_deep'], ['罗辑', '泰勒']);
    const entry = hg.entries[0];
    expect(entry.relatedTags).toEqual(['crisis_era_deep']);
    expect(entry.relatedPersons).toEqual(['罗辑', '泰勒']);
  });

  it('getEntriesByType 按类型筛选', () => {
    hg.recordMilestone(10, 0, '里程碑', '描述');
    hg.recordEvent(20, 1, '事件', '描述');
    hg.recordCrisis(30, 2, '危机', '描述');
    hg.recordVictory(400, 4, '胜利', '描述');
    hg.recordTagChange(15, 0, 'tag', '标签', true);

    expect(hg.getEntriesByType('MILESTONE').length).toBe(1);
    expect(hg.getEntriesByType('EVENT').length).toBe(1);
    expect(hg.getEntriesByType('CRISIS').length).toBe(1);
    expect(hg.getEntriesByType('VICTORY').length).toBe(1);
    expect(hg.getEntriesByType('TAG_APPLIED').length).toBe(1);
  });
});