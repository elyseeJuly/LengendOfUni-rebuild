import { describe, it, expect, beforeEach } from 'vitest';
import { Game } from '../../core/Game';
import { EpochType } from '../../types/enums';

describe('Game Core', () => {
  let game: Game;

  beforeEach(() => {
    game = new Game();
  });

  it('初始化年份为0', () => {
    expect(game.year).toBe(0);
  });

  it('初始纪元为危机', () => {
    expect(game.epoch).toBe(EpochType.CRISIS);
  });

  it('地球人口初始65', () => {
    expect(game.earthCivi.population).toBe(65);
  });

  it('Flag系统工作正常', () => {
    game.addFlag('test_flag');
    expect(game.hasFlag('test_flag')).toBe(true);
    game.removeFlag('test_flag');
    expect(game.hasFlag('test_flag')).toBe(false);
  });
});
