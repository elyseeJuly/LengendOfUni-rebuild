import { describe, it, expect, beforeEach } from 'vitest';
import { Game } from '../../core/Game';
import { STAR_INDEX } from '../../config/starIndices';
import { FriendshipType, TecTreeType } from '../../types/enums';

describe('Issue Resolutions and Refactors tests', () => {
  let game: Game;

  beforeEach(() => {
    game = new Game();
  });

  // Test 1: Relative Path CG mapping
  it('should resolve story CGs correctly regardless of leading slash', () => {
    // Relative path
    const cgUrl1 = (game.eventManager as any).mapAvatar("images/event_crisis_start.png");
    expect(cgUrl1).toContain("cg_crisis_start.png");

    // Absolute path
    const cgUrl2 = (game.eventManager as any).mapAvatar("/images/event_crisis_start.png");
    expect(cgUrl2).toContain("cg_crisis_start.png");
    
    // Normal name mapped
    const nameUrl = (game.eventManager as any).mapAvatar("luoji");
    expect(nameUrl).toContain("unified_luoji_");
  });

  // Test 2: Strategic Diplomacy Locks & Unlocks
  it('should unlock civilizations progressively based on tech and epoch rules', () => {
    const manager = game.alienCiviManager;
    const trisolaris = manager.aliens.get("三体");
    const singer = manager.aliens.get("歌者");
    const zeroers = manager.aliens.get("归零者");

    // Trisolaris is unlocked by default
    expect(trisolaris?.unlocked).toBe(true);
    // Others are locked by default
    expect(singer?.unlocked).toBe(false);
    expect(zeroers?.unlocked).toBe(false);

    // Simulate tech finished for Singer
    const node = game.earthCivi.tecTreeManager.trees.get(TecTreeType.PHYSICS)?.getNode("50光年远镜");
    if (node) {
      node.finished = true;
    }
    game.updateDiplomacyUnlocks();
    expect(singer?.unlocked).toBe(true);

    // Simulate year progress for Zeroers
    game.year = 280;
    game.updateDiplomacyUnlocks();
    expect(zeroers?.unlocked).toBe(true);
  });

  // Test 3: Trisolaris Deterrence diplomacy integration
  it('should check swordholder presence and modify deterrence value during Trisolaris diplomacy', () => {
    const trisolaris = game.alienCiviManager.aliens.get("三体");
    expect(trisolaris).toBeDefined();
    
    // 1. Provoke without swordholder should fail
    game.earthCivi.swordholder = null;
    let result = game.conductDiplomacy("三体", "provoke");
    expect(result).toContain("当前没有执剑人在位");
    expect(game.earthCivi.deterrenceValue).toBe(0);

    // Clear cooldown for test
    if (trisolaris) trisolaris.diplomacyCooldown = 0;

    // 2. Provoke with swordholder should succeed, increase deterrence
    game.earthCivi.swordholder = "罗辑";
    result = game.conductDiplomacy("三体", "provoke");
    expect(result).toContain("威慑度提升");
    expect(game.earthCivi.deterrenceValue).toBe(20);

    // Clear cooldown for test
    if (trisolaris) trisolaris.diplomacyCooldown = 0;

    // 3. Negotiate should decrease deterrence
    result = game.conductDiplomacy("三体", "negotiate");
    expect(result).toContain("外交和平谈判");
    expect(game.earthCivi.deterrenceValue).toBe(10);
  });
});
