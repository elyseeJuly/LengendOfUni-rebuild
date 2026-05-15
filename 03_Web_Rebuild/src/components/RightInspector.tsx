import React, { useState, useEffect } from 'react';
import { Target, Zap, ArrowUpCircle, Rocket, Factory, Pickaxe, Building, Gem, Skull, Crown } from 'lucide-react';
import { GameInstance } from '../core/Game';
import { Star } from '../core/Star';
import { createFleet } from '../core/Fleet';

export const RightInspector: React.FC = () => {
  const [selectedStar, setSelectedStar] = useState<Star | null>(null);
  const [, forceUpdate] = useState(0);

  useEffect(() => {
    const handleStarSelect = (e: Event) => {
      const customEvent = e as CustomEvent<Star>;
      setSelectedStar(customEvent.detail);
    };
    const handleTurnComplete = () => forceUpdate(n => n + 1);

    window.addEventListener('star-selected', handleStarSelect);
    window.addEventListener('game-turn-complete', handleTurnComplete);
    return () => {
      window.removeEventListener('star-selected', handleStarSelect);
      window.removeEventListener('game-turn-complete', handleTurnComplete);
    };
  }, []);

  const game = GameInstance.get();
  const star = selectedStar || (() => {
    const earth = game.starManager.getStar(4);
    return earth || null;
  })();

  if (!star) {
    return (
      <aside className="w-80 h-full glass-panel border-l border-white/5 flex flex-col p-6">
        <p className="text-[var(--text-secondary)]">选择一颗星球以查看详情</p>
      </aside>
    );
  }

  const isEarth = star.belongToCivi === "地球";
  const earth = game.earthCivi;

  const handleBuildStope = () => {
    if (!star.hasStope && earth.economy >= 30) {
      earth.economy -= 30;
      star.hasStope = true;
      game.addHistory(`在 ${star.name} 建造采矿场，消耗 30 经济。`);
      forceUpdate(n => n + 1);
    } else if (!star.hasStope) {
      alert("经济不足 30 点！");
    }
  };
  const handleBuildFactory = () => {
    if (!star.hasFactory && earth.economy >= 50) {
      earth.economy -= 50;
      star.hasFactory = true;
      game.addHistory(`在 ${star.name} 建造加工厂，消耗 50 经济。`);
      forceUpdate(n => n + 1);
    } else if (!star.hasFactory) {
      alert("经济不足 50 点！");
    }
  };
  const handleBuildCity = () => {
    if (!star.hasCity && earth.economy >= 80) {
      earth.economy -= 80;
      star.hasCity = true;
      game.addHistory(`在 ${star.name} 建造太空城市，消耗 80 经济。`);
      forceUpdate(n => n + 1);
    } else if (!star.hasCity) {
      alert("经济不足 80 点！");
    }
  };
  const handleBuildFleet = () => {
    if (earth.economy >= 100) {
      earth.economy -= 100;
      game.addHistory(`在 ${star.name} 开始建造恒星级战舰 10 艘！`);
      forceUpdate(n => n + 1);
      window.dispatchEvent(new CustomEvent('game-turn-complete'));
    } else {
      alert("经济不足 100 点！");
    }
  };
  const handleDispatchFleet = () => {
    const fleet = createFleet("地球第一舰队", "地球", star.index, 5, 3);
    fleet.leaderName = "章北海";
    fleet.weapons.push({ weaponName: "恒星级战舰", currentBuild: 10 });
    earth.fleets.push(fleet);
    game.addHistory(`【出征】组建 ${fleet.name} 离开 ${star.name}，目标木星，预计 3 回合后抵达。`);
    forceUpdate(n => n + 1);
    window.dispatchEvent(new CustomEvent('game-turn-complete'));
  };

  return (
    <aside className="w-80 h-full glass-panel border-l border-white/5 flex flex-col p-6 animate-in slide-in-from-right duration-300">
      <div className="flex flex-col gap-1 mb-6">
        <div className="text-[10px] font-bold text-[var(--text-secondary)] uppercase tracking-[0.2em]">
          Current Selection
        </div>
        <h2 className="text-2xl font-bold text-[var(--color-primary)]">{star.name}</h2>
        <div className="text-xs text-[var(--text-secondary)] flex items-center gap-1">
          <Target size={12} />
          <span>所属: {star.belongToCivi || "无"} | 资源: {star.currentResource}/{star.totalResource}</span>
        </div>
      </div>

      <div className="space-y-6 flex-1 overflow-y-auto pr-2">
        <section className="space-y-3">
          <h3 className="text-sm font-bold uppercase tracking-widest text-[var(--text-primary)]">星球概况</h3>
          <div className="grid grid-cols-2 gap-3">
            <div className="bg-black/5 dark:bg-white/5 p-2.5 rounded border border-white/5">
              <div className="text-[10px] text-[var(--text-secondary)] uppercase">人口上限</div>
              <div className="text-sm font-bold font-data">{star.populationLimit}</div>
            </div>
            <div className="bg-black/5 dark:bg-white/5 p-2.5 rounded border border-white/5">
              <div className="text-[10px] text-[var(--text-secondary)] uppercase">当前人口</div>
              <div className="text-sm font-bold font-data">{star.currentPopulation}</div>
            </div>
          </div>
        </section>

        {isEarth && (
          <>
            <section className="space-y-3">
              <h3 className="text-sm font-bold uppercase tracking-widest text-[var(--text-primary)]">文明状态</h3>
              <div className="space-y-3">
                <div className="bg-black/5 dark:bg-white/5 p-2.5 rounded border border-white/5">
                  <div className="flex items-center justify-between mb-1">
                    <div className="flex items-center gap-1.5">
                      <Crown size={11} className="text-yellow-400" />
                      <span className="text-[10px] text-[var(--text-secondary)] uppercase">文明等级</span>
                    </div>
                    <span className="text-xs font-bold text-yellow-400">{earth.getCiviLevelLabel()}</span>
                  </div>
                </div>
                <div className="bg-black/5 dark:bg-white/5 p-2.5 rounded border border-white/5">
                  <div className="flex items-center justify-between mb-1">
                    <div className="flex items-center gap-1.5">
                      <Gem size={11} className="text-cyan-400" />
                      <span className="text-[10px] text-[var(--text-secondary)] uppercase">资源储备</span>
                    </div>
                    <span className="text-xs font-bold text-cyan-400">{earth.resource}</span>
                  </div>
                </div>
                <div>
                  <div className="flex items-center justify-between mb-1">
                    <div className="flex items-center gap-1.5">
                      <Skull size={11} className={earth.treachery > 80 ? "text-red-500" : "text-[var(--text-secondary)]"} />
                      <span className="text-[10px] text-[var(--text-secondary)] uppercase">逃亡主义</span>
                    </div>
                    <span className={`text-xs font-bold ${earth.treachery > 80 ? "text-red-500" : ""}`}>{earth.treachery}%</span>
                  </div>
                  <div className="h-1.5 w-full bg-white/10 rounded-full overflow-hidden">
                    <div className={`h-full rounded-full transition-all ${earth.treachery > 80 ? 'bg-red-500' : 'bg-orange-500'}`} style={{ width: `${Math.min(earth.treachery, 100)}%` }} />
                  </div>
                </div>
              </div>
            </section>

            <section className="space-y-3">
              <h3 className="text-sm font-bold uppercase tracking-widest text-[var(--text-primary)]">工人分配</h3>
              <div className="space-y-2">
                <div className="flex justify-between text-[10px]">
                  <span className="text-[var(--text-secondary)]">采矿工人</span>
                  <span className="font-bold">{earth.miningWorkers}</span>
                </div>
                <div className="flex justify-between text-[10px]">
                  <span className="text-[var(--text-secondary)]">工厂工人</span>
                  <span className="font-bold">{earth.factoryWorkers}</span>
                </div>
                <div className="flex justify-between text-[10px]">
                  <span className="text-[var(--text-secondary)]">文化工人</span>
                  <span className="font-bold">{earth.cultureWorkers}</span>
                </div>
                <div className="flex justify-between text-[10px]">
                  <span className="text-[var(--text-secondary)]">闲置工人</span>
                  <span className="font-bold">{earth.idleWorkers}</span>
                </div>
              </div>
            </section>

            <section className="space-y-3">
              <h3 className="text-sm font-bold uppercase tracking-widest text-[var(--text-primary)]">基建设施</h3>
              <div className="space-y-2">
                <button onClick={handleBuildStope} className="w-full flex items-center justify-between p-3 rounded-lg border border-white/10 hover:border-[var(--color-primary)] hover:bg-[var(--color-primary)]/5 transition-all group">
                  <div className="flex items-center gap-3">
                    <Pickaxe className={star.hasStope ? "text-[var(--color-primary)]" : "text-[var(--text-secondary)]"} size={18} />
                    <div className="text-left">
                      <div className="text-sm font-bold">{star.hasStope ? '✅' : '➕'} 采矿场</div>
                      {!star.hasStope && <div className="text-[10px] text-[var(--text-secondary)]">消耗 30 经济</div>}
                    </div>
                  </div>
                  {!star.hasStope && <ArrowUpCircle className="opacity-0 group-hover:opacity-100 transition-opacity" size={16} />}
                </button>
                <button onClick={handleBuildFactory} className="w-full flex items-center justify-between p-3 rounded-lg border border-white/10 hover:border-[var(--color-primary)] hover:bg-[var(--color-primary)]/5 transition-all group">
                  <div className="flex items-center gap-3">
                    <Factory className={star.hasFactory ? "text-[var(--color-primary)]" : "text-[var(--text-secondary)]"} size={18} />
                    <div className="text-left">
                      <div className="text-sm font-bold">{star.hasFactory ? '✅' : '➕'} 加工厂</div>
                      {!star.hasFactory && <div className="text-[10px] text-[var(--text-secondary)]">消耗 50 经济</div>}
                    </div>
                  </div>
                  {!star.hasFactory && <ArrowUpCircle className="opacity-0 group-hover:opacity-100 transition-opacity" size={16} />}
                </button>
                <button onClick={handleBuildCity} className="w-full flex items-center justify-between p-3 rounded-lg border border-white/10 hover:border-[var(--color-primary)] hover:bg-[var(--color-primary)]/5 transition-all group">
                  <div className="flex items-center gap-3">
                    <Building className={star.hasCity ? "text-[var(--color-primary)]" : "text-[var(--text-secondary)]"} size={18} />
                    <div className="text-left">
                      <div className="text-sm font-bold">{star.hasCity ? '✅' : '➕'} 太空城市</div>
                      {!star.hasCity && <div className="text-[10px] text-[var(--text-secondary)]">消耗 80 经济</div>}
                    </div>
                  </div>
                  {!star.hasCity && <ArrowUpCircle className="opacity-0 group-hover:opacity-100 transition-opacity" size={16} />}
                </button>
              </div>
            </section>

            <section className="space-y-3">
              <h3 className="text-sm font-bold uppercase tracking-widest text-[var(--text-primary)]">军工与舰队</h3>
              <div className="space-y-2">
                <button onClick={handleBuildFleet} className="w-full flex items-center justify-between p-3 rounded-lg border border-[var(--color-primary)]/20 bg-[var(--color-primary)]/5 hover:bg-[var(--color-primary)]/10 transition-all">
                  <div className="flex items-center gap-3">
                    <Zap className="text-[var(--color-primary)]" size={18} />
                    <div className="text-left">
                      <div className="text-sm font-bold">建造战舰 (10 艘)</div>
                      <div className="text-[10px] text-[var(--text-secondary)]">消耗 100 经济</div>
                    </div>
                  </div>
                </button>
                <button onClick={handleDispatchFleet} className="w-full flex items-center justify-between p-3 rounded-lg border border-orange-500/30 hover:bg-orange-500/10 transition-all">
                  <div className="flex items-center gap-3">
                    <Rocket className="text-orange-500" size={18} />
                    <div className="text-left">
                      <div className="text-sm font-bold text-orange-400">🚀 派遣舰队</div>
                      <div className="text-[10px] text-[var(--text-secondary)]">目标: 木星</div>
                    </div>
                  </div>
                </button>
              </div>
            </section>
          </>
        )}
      </div>

      <div className="mt-auto pt-6 border-t border-white/5">
        <div className="flex justify-between items-center mb-4">
          <span className="text-xs font-bold uppercase text-[var(--text-secondary)]">舰队数</span>
          <span className="text-xs font-data font-bold">{earth.fleets.length}</span>
        </div>
        <div className="h-1.5 w-full bg-white/10 rounded-full overflow-hidden">
          <div className="h-full bg-[var(--color-primary)] rounded-full" style={{ width: `${Math.min(earth.deterrenceValue, 100)}%` }} />
        </div>
        <div className="text-[10px] text-right text-[var(--text-secondary)] mt-1">威慑度: {Math.floor(earth.deterrenceValue)}%</div>
      </div>
    </aside>
  );
};
