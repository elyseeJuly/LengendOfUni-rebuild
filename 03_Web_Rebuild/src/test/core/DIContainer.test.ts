import { describe, it, expect, beforeEach } from 'vitest';
import { DIContainer, ServiceKeys } from '../../core/DIContainer';

describe('DIContainer', () => {
  let container: DIContainer;

  beforeEach(() => {
    container = new DIContainer();
  });

  it('register 存储实例', () => {
    const instance = { name: 'test' };
    container.register('testService', instance);
    expect(container.has('testService')).toBe(true);
  });

  it('resolve 返回注册的实例', () => {
    const instance = { value: 42 };
    container.register('myService', instance);
    const resolved = container.resolve<{ value: number }>('myService');
    expect(resolved.value).toBe(42);
    expect(resolved).toBe(instance);
  });

  it('resolve 未注册的键抛出错误', () => {
    expect(() => container.resolve('unregistered')).toThrow('not registered');
  });

  it('registerFactory 注册工厂', () => {
    container.registerFactory('factoryService', () => ({ id: Math.random() }));
    expect(container.has('factoryService')).toBe(true);
  });

  it('singleton factory 只创建一次', () => {
    let createCount = 0;
    container.registerFactory('singleton', () => {
      createCount++;
      return { id: createCount };
    });
    const a = container.resolve('singleton');
    const b = container.resolve('singleton');
    expect(a).toBe(b);
    expect(createCount).toBe(1);
  });

  it('non-singleton factory 每次创建新实例', () => {
    let createCount = 0;
    container.registerFactory('nonSingleton', () => {
      createCount++;
      return { id: createCount };
    }, false);
    const a = container.resolve('nonSingleton');
    const b = container.resolve('nonSingleton');
    expect(a).not.toBe(b);
    expect(a.id).toBe(1);
    expect(b.id).toBe(2);
  });

  it('has 返回正确布尔值', () => {
    expect(container.has('nonexistent')).toBe(false);
    container.register('svc', {});
    expect(container.has('svc')).toBe(true);
    container.registerFactory('factory', () => ({}));
    expect(container.has('factory')).toBe(true);
  });

  it('remove 删除服务', () => {
    container.register('svc', { data: 1 });
    expect(container.has('svc')).toBe(true);
    container.remove('svc');
    expect(container.has('svc')).toBe(false);
    expect(() => container.resolve('svc')).toThrow();
  });

  it('remove 删除不存在的服务不报错', () => {
    expect(() => container.remove('nonexistent')).not.toThrow();
  });

  it('clear 清空容器', () => {
    container.register('svc1', {});
    container.register('svc2', {});
    container.registerFactory('factory', () => ({}));
    container.clear();
    expect(container.has('svc1')).toBe(false);
    expect(container.has('svc2')).toBe(false);
    expect(container.has('factory')).toBe(false);
    expect(container.getRegisteredKeys()).toEqual([]);
  });

  it('getRegisteredKeys 返回所有键', () => {
    container.register('svc1', {});
    container.registerFactory('factory1', () => ({}));
    const keys = container.getRegisteredKeys();
    expect(keys).toContain('svc1');
    expect(keys).toContain('factory1');
    expect(keys.length).toBe(2);
  });

  it('ServiceKeys 包含所有预期常量', () => {
    const expectedKeys = [
      'game', 'eventManager', 'eventBus', 'tagManager',
      'atmosphereEngine', 'historyGenerator', 'saveManager',
      'audioManager', 'combatEngine', 'ecologyChain', 'relationNetwork',
    ];
    const serviceKeyValues = Object.values(ServiceKeys);
    for (const key of expectedKeys) {
      expect(serviceKeyValues).toContain(key);
    }
  });

  it('register 可以覆盖已存在的服务', () => {
    container.register('svc', { version: 1 });
    container.register('svc', { version: 2 });
    const resolved = container.resolve<{ version: number }>('svc');
    expect(resolved.version).toBe(2);
  });

  it('复杂类型注册和解析', () => {
    interface ComplexService {
      id: string;
      data: number[];
      process: (x: number) => number;
    }
    const service: ComplexService = {
      id: 'complex',
      data: [1, 2, 3],
      process: (x: number) => x * 2,
    };
    container.register('complex', service);
    const resolved = container.resolve<ComplexService>('complex');
    expect(resolved.id).toBe('complex');
    expect(resolved.data).toEqual([1, 2, 3]);
    expect(resolved.process(5)).toBe(10);
  });

  it('factory 创建复杂实例', () => {
    container.registerFactory('gameService', () => ({
      name: 'Game',
      state: 'running',
      tick: () => 'tick',
    }));
    const svc = container.resolve<{ name: string; state: string; tick: () => string }>('gameService');
    expect(svc.name).toBe('Game');
    expect(svc.tick()).toBe('tick');
  });
});