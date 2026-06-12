import { describe, it, expect, beforeEach, vi } from 'vitest';
import { AtmosphereEngine, AtmosphereState } from '../../core/AtmosphereEngine';
import { TagManager } from '../../core/TagManager';
import { EarthCivilization } from '../../core/EarthCivilization';

describe('AtmosphereEngine', () => {
  let ae: AtmosphereEngine;
  let tagManager: TagManager;
  let earthCivi: EarthCivilization;

  beforeEach(() => {
    ae = new AtmosphereEngine();
    tagManager = new TagManager();
    earthCivi = new EarthCivilization();
  });

  it('初始状态是 NORMAL', () => {
    expect(ae.currentState).toBe('NORMAL');
  });

  it('getConfig 返回 NORMAL 状态的配置', () => {
    const config = ae.getConfig();
    expect(config.state).toBe('NORMAL');
    expect(config.backgroundColor).toBe('#0a0a1a');
    expect(config.label).toBe('正常');
  });

  it('getConfig 切换后返回正确配置', () => {
    ae.transitionTo('CRITICAL');
    const config = ae.getConfig();
    expect(config.state).toBe('CRITICAL');
    expect(config.backgroundColor).toBe('#1a0000');
  });

  it('evaluate 当 population < 10 时返回 CRITICAL', () => {
    earthCivi.population = 5;
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('CRITICAL');
  });

  it('evaluate 当 economy < 20 时返回 CRITICAL', () => {
    earthCivi.population = 100;
    earthCivi.economy = 10;
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('CRITICAL');
  });

  it('evaluate civil_unrest > 60 时返回 TENSE', () => {
    earthCivi.population = 100;
    earthCivi.economy = 50;
    tagManager.applyWorldTag('civil_unrest', 70, 'test', 10);
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('TENSE');
  });

  it('evaluate foil_imminent > 50 时返回 DARK', () => {
    earthCivi.population = 100;
    earthCivi.economy = 50;
    tagManager.applyWorldTag('foil_imminent', 80, 'test', 10);
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('DARK');
  });

  it('evaluate digital_religion > 80 时返回 TRANSCENDENT', () => {
    earthCivi.population = 100;
    earthCivi.economy = 50;
    tagManager.applyWorldTag('digital_religion', 90, 'test', 10);
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('TRANSCENDENT');
  });

  it('evaluate tech_boom 存在时返回 HOPEFUL', () => {
    earthCivi.population = 100;
    earthCivi.economy = 50;
    tagManager.applyWorldTag('tech_boom', 50, 'test', 10);
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('HOPEFUL');
  });

  it('evaluate space_force_built 存在时返回 HOPEFUL', () => {
    earthCivi.population = 100;
    earthCivi.economy = 50;
    tagManager.applyWorldTag('space_force_built', 100, 'test', 10);
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('HOPEFUL');
  });

  it('evaluate 无触发条件时返回 NORMAL', () => {
    earthCivi.population = 100;
    earthCivi.economy = 50;
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('NORMAL');
  });

  it('evaluate 优先级: TRANSCENDENT > DARK > CRITICAL > TENSE > HOPEFUL > NORMAL', () => {
    earthCivi.population = 5;
    tagManager.applyWorldTag('digital_religion', 90, 'test', 10);
    const result = ae.evaluate(tagManager, earthCivi);
    expect(result).toBe('TRANSCENDENT');
  });

  it('transitionTo 改变状态并返回 true', () => {
    const changed = ae.transitionTo('TENSE');
    expect(changed).toBe(true);
    expect(ae.currentState).toBe('TENSE');
  });

  it('transitionTo 相同状态不切换返回 false', () => {
    const changed = ae.transitionTo('NORMAL');
    expect(changed).toBe(false);
    expect(ae.currentState).toBe('NORMAL');
  });

  it('transitionTo 切换时派发事件', () => {
    const dispatchSpy = vi.spyOn(window, 'dispatchEvent');
    ae.transitionTo('HOPEFUL');
    expect(dispatchSpy).toHaveBeenCalled();
    const event = dispatchSpy.mock.calls[0][0] as CustomEvent;
    expect(event.type).toBe('game:atmosphere:changed');
    expect(event.detail.state).toBe('HOPEFUL');
    dispatchSpy.mockRestore();
  });

  it('transitionTo 相同状态不派发事件', () => {
    const dispatchSpy = vi.spyOn(window, 'dispatchEvent');
    ae.transitionTo('NORMAL');
    expect(dispatchSpy).not.toHaveBeenCalled();
    dispatchSpy.mockRestore();
  });

  it('getConfigForState 返回指定状态配置', () => {
    const config = ae.getConfigForState('DARK');
    expect(config.state).toBe('DARK');
    expect(config.backgroundColor).toBe('#000010');
    expect(config.noiseLevel).toBe(0.15);
    expect(config.vignetteIntensity).toBe(0.5);
    expect(config.transitionMs).toBe(3000);
  });

  it('getConfigForState 所有状态都有配置', () => {
    const states: AtmosphereState[] = ['NORMAL', 'TENSE', 'CRITICAL', 'DARK', 'HOPEFUL', 'TRANSCENDENT'];
    for (const state of states) {
      const config = ae.getConfigForState(state);
      expect(config.state).toBe(state);
      expect(typeof config.backgroundColor).toBe('string');
      expect(typeof config.uiTint).toBe('string');
      expect(typeof config.noiseLevel).toBe('number');
      expect(typeof config.transitionMs).toBe('number');
      expect(typeof config.label).toBe('string');
    }
  });

  it('toJSON/fromJSON 序列化往返', () => {
    ae.transitionTo('CRITICAL');
    const json = ae.toJSON();
    const restored = AtmosphereEngine.fromJSON(json);
    expect(restored.currentState).toBe('CRITICAL');
  });

  it('fromJSON 空数据使用默认 NORMAL', () => {
    const restored = AtmosphereEngine.fromJSON({});
    expect(restored.currentState).toBe('NORMAL');
  });

  it('fromJSON null 使用默认 NORMAL', () => {
    const restored = AtmosphereEngine.fromJSON(null);
    expect(restored.currentState).toBe('NORMAL');
  });

  it('所有氛围状态配置包含必要字段', () => {
    const states: AtmosphereState[] = ['NORMAL', 'TENSE', 'CRITICAL', 'DARK', 'HOPEFUL', 'TRANSCENDENT'];
    for (const state of states) {
      const config = ae.getConfigForState(state);
      expect(config).toHaveProperty('state');
      expect(config).toHaveProperty('backgroundColor');
      expect(config).toHaveProperty('uiTint');
      expect(config).toHaveProperty('noiseLevel');
      expect(config).toHaveProperty('scanlineOpacity');
      expect(config).toHaveProperty('vignetteIntensity');
      expect(config).toHaveProperty('textGlowColor');
      expect(config).toHaveProperty('transitionMs');
      expect(config).toHaveProperty('label');
      expect(config).toHaveProperty('description');
    }
  });
});