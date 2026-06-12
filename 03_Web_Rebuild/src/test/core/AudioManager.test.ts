import { describe, it, expect, beforeEach } from 'vitest';
import { AudioManager } from '../../core/AudioManager';

describe('AudioManager', () => {
  let am: AudioManager;

  beforeEach(() => {
    am = new AudioManager();
  });

  it('init 不抛出异常', () => {
    expect(() => am.init()).not.toThrow();
  });

  it('重复 init 不报错', () => {
    am.init();
    expect(() => am.init()).not.toThrow();
  });

  it('初始 bgmEnabled 为 true', () => {
    expect(am.getState().bgmEnabled).toBe(true);
  });

  it('初始 sfxEnabled 为 true', () => {
    expect(am.getState().sfxEnabled).toBe(true);
  });

  it('初始 bgmVolume 为 0.5', () => {
    expect(am.getState().bgmVolume).toBe(0.5);
  });

  it('初始 sfxVolume 为 0.7', () => {
    expect(am.getState().sfxVolume).toBe(0.7);
  });

  it('初始 currentBgm 为 null', () => {
    expect(am.getState().currentBgm).toBeNull();
  });

  it('setBgmVolume 设置值', () => {
    am.setBgmVolume(0.8);
    expect(am.getState().bgmVolume).toBe(0.8);
  });

  it('setBgmVolume 限制下限为 0', () => {
    am.setBgmVolume(-0.5);
    expect(am.getState().bgmVolume).toBe(0);
  });

  it('setBgmVolume 限制上限为 1', () => {
    am.setBgmVolume(2.0);
    expect(am.getState().bgmVolume).toBe(1);
  });

  it('setSfxVolume 设置值', () => {
    am.setSfxVolume(0.3);
    expect(am.getState().sfxVolume).toBe(0.3);
  });

  it('setSfxVolume 限制下限为 0', () => {
    am.setSfxVolume(-1.0);
    expect(am.getState().sfxVolume).toBe(0);
  });

  it('setSfxVolume 限制上限为 1', () => {
    am.setSfxVolume(1.5);
    expect(am.getState().sfxVolume).toBe(1);
  });

  it('toggleBgm 切换状态', () => {
    const initialState = am.getState().bgmEnabled;
    const newState = am.toggleBgm();
    expect(newState).toBe(!initialState);
    expect(am.getState().bgmEnabled).toBe(!initialState);
    const toggledBack = am.toggleBgm();
    expect(toggledBack).toBe(initialState);
    expect(am.getState().bgmEnabled).toBe(initialState);
  });

  it('toggleSfx 切换状态', () => {
    const initialState = am.getState().sfxEnabled;
    const newState = am.toggleSfx();
    expect(newState).toBe(!initialState);
    expect(am.getState().sfxEnabled).toBe(!initialState);
    const toggledBack = am.toggleSfx();
    expect(toggledBack).toBe(initialState);
    expect(am.getState().sfxEnabled).toBe(initialState);
  });

  it('getState 返回当前状态', () => {
    am.setBgmVolume(0.6);
    am.setSfxVolume(0.4);
    am.toggleSfx();
    const state = am.getState();
    expect(state.bgmVolume).toBe(0.6);
    expect(state.sfxVolume).toBe(0.4);
    expect(state.sfxEnabled).toBe(false);
    expect(state.bgmEnabled).toBe(true);
  });

  it('destroy 清理音频上下文', () => {
    am.init();
    am.destroy();
    expect(am.getAudioContext()).toBeNull();
  });

  it('destroy 多次调用不报错', () => {
    am.destroy();
    expect(() => am.destroy()).not.toThrow();
  });

  it('getState.bgmVolume 返回设置的值', () => {
    am.setBgmVolume(0.25);
    expect(am.getState().bgmVolume).toBe(0.25);
    am.setBgmVolume(0.75);
    expect(am.getState().bgmVolume).toBe(0.75);
  });

  it('getState.sfxVolume 返回设置的值', () => {
    am.setSfxVolume(0.15);
    expect(am.getState().sfxVolume).toBe(0.15);
    am.setSfxVolume(0.95);
    expect(am.getState().sfxVolume).toBe(0.95);
  });

  it('setMasterVolume 限制 0-1', () => {
    expect(() => am.setMasterVolume(0.5)).not.toThrow();
    expect(() => am.setMasterVolume(-1)).not.toThrow();
    expect(() => am.setMasterVolume(2)).not.toThrow();
  });

  it('未初始化时播放不报错', () => {
    expect(() => am.playClick()).not.toThrow();
    expect(() => am.playAlarm()).not.toThrow();
    expect(() => am.playMilestone()).not.toThrow();
    expect(() => am.playDissonance()).not.toThrow();
  });

  it('初始化后播放不报错', () => {
    am.init();
    expect(() => am.playClick()).not.toThrow();
    expect(() => am.playAlarm()).not.toThrow();
    expect(() => am.playMilestone()).not.toThrow();
    expect(() => am.playDissonance()).not.toThrow();
  });
});