import { describe, it, expect, beforeEach, vi } from 'vitest';
import { EventBus, GameEvents } from '../../core/EventBus';

describe('EventBus', () => {
  let bus: EventBus;

  beforeEach(() => {
    bus = new EventBus();
  });

  it('on 注册处理器', () => {
    let called = false;
    bus.on('test:event', () => { called = true; });
    bus.emit('test:event');
    expect(called).toBe(true);
  });

  it('off 取消注册处理器', () => {
    let callCount = 0;
    const handler = () => { callCount++; };
    bus.on('test:event', handler);
    bus.off('test:event', handler);
    bus.emit('test:event');
    expect(callCount).toBe(0);
  });

  it('off 不存在的处理器不报错', () => {
    const handler = () => {};
    expect(() => bus.off('nonexistent', handler)).not.toThrow();
  });

  it('emit 调用所有注册处理器', () => {
    let count1 = 0, count2 = 0;
    bus.on('test:event', () => { count1++; });
    bus.on('test:event', () => { count2++; });
    bus.emit('test:event');
    expect(count1).toBe(1);
    expect(count2).toBe(1);
  });

  it('emit 传递参数', () => {
    let receivedArgs: any[] = [];
    bus.on('test:event', (...args: any[]) => { receivedArgs = args; });
    bus.emit('test:event', 'arg1', 42, { key: 'value' });
    expect(receivedArgs).toEqual(['arg1', 42, { key: 'value' }]);
  });

  it('listenerCount 返回正确计数', () => {
    expect(bus.listenerCount('test:event')).toBe(0);
    const h1 = () => {};
    const h2 = () => {};
    bus.on('test:event', h1);
    expect(bus.listenerCount('test:event')).toBe(1);
    bus.on('test:event', h2);
    expect(bus.listenerCount('test:event')).toBe(2);
    bus.off('test:event', h1);
    expect(bus.listenerCount('test:event')).toBe(1);
  });

  it('clear 移除所有处理器', () => {
    bus.on('event1', () => {});
    bus.on('event2', () => {});
    bus.on('event3', () => {});
    expect(bus.listenerCount('event1')).toBe(1);
    bus.clear();
    expect(bus.listenerCount('event1')).toBe(0);
    expect(bus.listenerCount('event2')).toBe(0);
    expect(bus.listenerCount('event3')).toBe(0);
  });

  it('emitToWindow 派发 CustomEvent 到 window', () => {
    const dispatchSpy = vi.spyOn(window, 'dispatchEvent');
    bus.emitToWindow('game:test', { data: 123 });
    expect(dispatchSpy).toHaveBeenCalled();
    const event = dispatchSpy.mock.calls[0][0] as CustomEvent;
    expect(event.type).toBe('game:test');
    expect(event.detail).toEqual({ data: 123 });
    dispatchSpy.mockRestore();
  });

  it('emitToWindow 同时调用总线处理器', () => {
    let busCalled = false;
    bus.on('game:test', () => { busCalled = true; });
    bus.emitToWindow('game:test', {});
    expect(busCalled).toBe(true);
  });

  it('处理器内抛出错误不影响其他处理器', () => {
    let secondCalled = false;
    const consoleSpy = vi.spyOn(console, 'error').mockImplementation(() => {});
    bus.on('test:event', () => { throw new Error('Handler error'); });
    bus.on('test:event', () => { secondCalled = true; });
    expect(() => bus.emit('test:event')).not.toThrow();
    expect(secondCalled).toBe(true);
    consoleSpy.mockRestore();
  });

  it('GameEvents 是字符串常量', () => {
    expect(GameEvents.TURN_START).toBe('game:turn:start');
    expect(GameEvents.TURN_COMPLETE).toBe('game:turn:complete');
    expect(GameEvents.EPOCH_CHANGED).toBe('game:epoch:changed');
    expect(GameEvents.EVENT_TRIGGERED).toBe('game:event:triggered');
    expect(GameEvents.TAG_APPLIED).toBe('game:tag:applied');
    expect(GameEvents.TAG_REMOVED).toBe('game:tag:removed');
    expect(GameEvents.ATMOSPHERE_CHANGED).toBe('game:atmosphere:changed');
    expect(GameEvents.BATTLE_START).toBe('game:battle:start');
    expect(GameEvents.BATTLE_END).toBe('game:battle:end');
    expect(GameEvents.GAME_OVER).toBe('game:over');
  });

  it('GameEvents 包含所有事件', () => {
    const allEvents = [
      'game:turn:start', 'game:turn:complete', 'game:epoch:changed',
      'game:event:triggered', 'game:tag:applied', 'game:tag:removed',
      'game:atmosphere:changed', 'game:battle:start', 'game:battle:end',
      'game:person:unlocked', 'game:save:completed', 'game:load:completed',
      'game:over', 'audio:play', 'audio:stop', 'game:resource:changed',
      'game:fleet:moved', 'game:tech:completed',
    ];
    for (const evt of allEvents) {
      expect(Object.values(GameEvents)).toContain(evt);
    }
  });

  it('toJSON 返回监听器计数', () => {
    bus.on('e1', () => {});
    bus.on('e1', () => {});
    bus.on('e2', () => {});
    const json = bus.toJSON() as { listenerCounts: [string, number][] };
    expect(json.listenerCounts).toBeDefined();
    const e1Count = json.listenerCounts.find(([k]) => k === 'e1')![1];
    const e2Count = json.listenerCounts.find(([k]) => k === 'e2')![1];
    expect(e1Count).toBe(2);
    expect(e2Count).toBe(1);
  });

  it('emit 不存在的处理器不报错', () => {
    expect(() => bus.emit('nonexistent')).not.toThrow();
  });

  it('同一个事件可注册多个处理器并全部调用', () => {
    const results: number[] = [];
    bus.on('calc', (n: number) => { results.push(n * 2); });
    bus.on('calc', (n: number) => { results.push(n * 3); });
    bus.emit('calc', 5);
    expect(results).toEqual([10, 15]);
  });
});