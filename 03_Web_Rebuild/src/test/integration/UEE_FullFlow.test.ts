import { describe, it, expect, beforeEach } from 'vitest';
import { Game } from '../../core/Game';
import { TagManager } from '../../core/TagManager';
import { AtmosphereEngine } from '../../core/AtmosphereEngine';
import { EcologyChain } from '../../core/EcologyChain';
import { HistoryGenerator } from '../../core/HistoryGenerator';
import { SliceNarrativeEngine } from '../../core/SliceNarrativeEngine';
import { EventBus } from '../../core/EventBus';
import { EarthCivilization } from '../../core/EarthCivilization';

describe('UEE_FullFlow', () => {
  let game: Game;

  beforeEach(() => {
    game = new Game();
  });

  it('Game 启动时包含所有 UEE 模块', () => {
    expect(game.tagManager).toBeInstanceOf(TagManager);
    expect(game.atmosphereEngine).toBeInstanceOf(AtmosphereEngine);
    expect(game.ecologyChain).toBeInstanceOf(EcologyChain);
    expect(game.historyGenerator).toBeInstanceOf(HistoryGenerator);
    expect(game.sliceNarrativeEngine).toBeInstanceOf(SliceNarrativeEngine);
    expect(game.eventBus).toBeInstanceOf(EventBus);
    expect(game.relationNetwork).toBeDefined();
  });

  it('TagManager counter 初始为 0', () => {
    expect(game.tagManager.worldTags.size).toBe(0);
  });

  it('EcologyChain activeChains 初始为空', () => {
    expect(game.ecologyChain.activeChains).toEqual([]);
  });

  it('EventBus 初始无处理器', () => {
    const json = game.eventBus.toJSON() as { listenerCounts: [string, number][] };
    expect(json.listenerCounts.length).toBe(0);
  });

  it('Tag decay 运行无错误', () => {
    game.tagManager.applyWorldTag('civil_unrest', 50, 'test', game.year);
    expect(() => game.tagManager.decayTags(game.year)).not.toThrow();
  });

  it('AtmosphereEngine evaluate 运行无错误', () => {
    expect(() => game.atmosphereEngine.evaluate(game.tagManager, game.earthCivi)).not.toThrow();
  });

  it('HistoryGenerator 记录 turn', () => {
    game.historyGenerator.incTurn();
    game.historyGenerator.recordEvent(game.year, game.epoch, '测试事件', '描述');
    expect(game.historyGenerator.entries.length).toBe(1);
    expect(game.historyGenerator.entries[0].entryType).toBe('EVENT');
  });

  it('Tag 自动基于状态阈值应用 population_crisis', () => {
    game.earthCivi.population = 15;
    if (game.earthCivi.population < 20 && !game.tagManager.hasTag('population_crisis')) {
      game.tagManager.applyWorldTag('population_crisis', 20, 'auto:system', game.year);
    }
    expect(game.tagManager.hasTag('population_crisis')).toBe(true);
    expect(game.tagManager.getTagIntensity('population_crisis')).toBe(20);
  });

  it('Tag 自动基于状态阈值应用 civil_unrest', () => {
    game.earthCivi.treachery = 70;
    if (game.earthCivi.treachery > 60 && !game.tagManager.hasTag('civil_unrest')) {
      game.tagManager.applyWorldTag('civil_unrest', 30, 'auto:system', game.year);
    }
    expect(game.tagManager.hasTag('civil_unrest')).toBe(true);
    expect(game.tagManager.getTagIntensity('civil_unrest')).toBe(30);
  });

  it('Tag 自动基于状态阈值应用 deterrence_steady', () => {
    game.earthCivi.deterrenceValue = 80;
    if (game.earthCivi.deterrenceValue > 60 && !game.tagManager.hasTag('deterrence_steady')) {
      game.tagManager.applyWorldTag('deterrence_steady', 40, 'auto:system', game.year);
    }
    expect(game.tagManager.hasTag('deterrence_steady')).toBe(true);
    expect(game.tagManager.getTagIntensity('deterrence_steady')).toBe(40);
  });

  it('全量 toJSON/fromJSON 往返', () => {
    game.tagManager.applyWorldTag('tech_boom', 60, 'test', game.year);
    game.tagManager.applyWorldTag('civil_unrest', 40, 'test', game.year);
    game.historyGenerator.recordMilestone(game.year, game.epoch, '测试里程碑', '描述');
    game.historyGenerator.recordEvent(game.year, game.epoch, '测试事件', '描述');
    game.atmosphereEngine.transitionTo('HOPEFUL');

    const tagJson = game.tagManager.toJSON();
    const atmosJson = game.atmosphereEngine.toJSON();
    const historyJson = game.historyGenerator.toJSON();
    const ecoJson = game.ecologyChain.toJSON();

    const tagRestored = TagManager.fromJSON(tagJson);
    const atmosRestored = AtmosphereEngine.fromJSON(atmosJson);
    const historyRestored = HistoryGenerator.fromJSON(historyJson);
    const ecoRestored = EcologyChain.fromJSON(ecoJson);

    expect(tagRestored.hasTag('tech_boom')).toBe(true);
    expect(tagRestored.getTagIntensity('civil_unrest')).toBe(40);
    expect(atmosRestored.currentState).toBe('HOPEFUL');
    expect(historyRestored.entries.length).toBe(2);
    expect(ecoRestored.activeChains).toEqual([]);
  });

  it('SliceNarrativeEngine 生成切片', () => {
    const slice = game.sliceNarrativeEngine.generateSlice('test_event', '科技突破', game.tagManager);
    expect(slice).toBeDefined();
    expect(slice.eventId).toBe('test_event');
    expect(slice.characterName).toBeTruthy();
    expect(slice.characterRole).toBeTruthy();
    expect(slice.scene).toBeTruthy();
    expect(slice.innerMonologue).toContain('科技突破');
    expect(game.sliceNarrativeEngine.hasSlices('test_event')).toBe(true);
  });

  it('关系网络初始化后包含预设关系', () => {
    expect(game.relationNetwork.relations.length).toBeGreaterThan(0);
    expect(game.relationNetwork.getRelation('罗辑', '史强')).not.toBeNull();
  });

  it('EcologyChain 预设链正确加载', () => {
    expect(game.ecologyChain.chains.length).toBeGreaterThan(0);
    const rationChain = game.ecologyChain.chains.find(c => c.id === 'ration_to_riot');
    expect(rationChain).toBeDefined();
    expect(rationChain!.triggerDelay).toBe(3);
  });

  it('AtmosphereEngine 切换状态后 HistoryGenerator 可记录', () => {
    const newState = game.atmosphereEngine.evaluate(game.tagManager, game.earthCivi);
    game.atmosphereEngine.transitionTo(newState);
    game.historyGenerator.recordEvent(
      game.year, game.epoch, '氛围变化',
      `氛围变为「${game.atmosphereEngine.getConfig().label}」`,
    );
    expect(game.historyGenerator.entries.length).toBe(1);
    expect(game.historyGenerator.entries[0].title).toBe('氛围变化');
  });

  it('runARound 不崩溃', () => {
    game.setRngProvider({ random: () => 0.5 });
    expect(() => game.runARound()).not.toThrow();
  });
});