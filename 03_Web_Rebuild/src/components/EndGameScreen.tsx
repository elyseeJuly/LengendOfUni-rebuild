import React, { useEffect, useState } from 'react';
import { GameInstance } from '../core/Game';
import { RefreshCw, BookOpen, AlertTriangle } from 'lucide-react';
import { TimelineComparisonPanel } from './TimelineComparisonPanel';

export const EndGameScreen: React.FC = () => {
  const game = GameInstance.get();
  const [logs, setLogs] = useState<string[]>([]);

  useEffect(() => {
    // Typewriter effect for history summary
    let i = 0;
    const history = game.historyLogs.slice(-15); // Show last 15 major events
    const timer = setInterval(() => {
      if (i < history.length) {
        setLogs(prev => [...prev, history[i]]);
        i++;
      } else {
        clearInterval(timer);
      }
    }, 200);
    return () => clearInterval(timer);
  }, [game.historyLogs]);

  return (
    <div className="fixed inset-0 z-[200] bg-[#050A1F] flex flex-col items-center justify-center p-8 overflow-hidden">
      {/* Background Effect */}
      <div className="absolute inset-0 opacity-20 pointer-events-none">
        <div className="absolute inset-0 bg-[radial-gradient(circle_at_center,_rgba(255,87,34,0.2)_0%,_transparent_70%)] animate-pulse" />
      </div>

      <div className="relative z-10 w-full max-w-4xl flex flex-col items-center text-center">
        <div className="mb-8 animate-bounce">
          <AlertTriangle size={64} className="text-forest-danger" />
        </div>
        
        <h1 className="text-6xl font-bold tracking-[0.3em] text-forest-danger mb-4 uppercase drop-shadow-[0_0_30px_rgba(255,87,34,0.5)]">
          文明终结
        </h1>
        
        <div className="h-1 w-64 bg-forest-danger/30 mb-8" />
        
        <p className="text-2xl font-medium text-forest-text mb-12 max-w-2xl leading-relaxed italic">
          "{game.gameOverReason}"
        </p>

        {/* History Summary:岁月史书 */}
        <div className="w-full bg-black/40 border border-white/5 p-6 rounded-sm mb-12 text-left h-[300px] overflow-y-auto font-data custom-scrollbar">
          <div className="flex items-center gap-2 mb-4 text-xs font-bold text-white/30 uppercase tracking-widest border-b border-white/5 pb-2">
            <BookOpen size={14} />
            <span>岁月史书: 文明历程总结</span>
          </div>
          <div className="space-y-2">
            {logs.map((log, idx) => (
              <div key={idx} className="text-sm text-forest-text-muted animate-in fade-in slide-in-from-left-4 duration-500">
                <span className="text-forest-primary/50 mr-2">&gt;&gt;&gt;</span>
                {log}
              </div>
            ))}
          </div>
        </div>

        {/* 双轨纪元对比 */}
        <TimelineComparisonPanel />

        <div className="flex gap-6">
          <button 
            onClick={() => window.location.reload()}
            className="group flex items-center gap-3 px-10 py-4 bg-forest-danger/20 hover:bg-forest-danger/40 border border-forest-danger/50 text-forest-danger transition-all rounded-sm font-bold uppercase tracking-widest"
          >
            <RefreshCw size={20} className="group-hover:rotate-180 transition-transform duration-700" />
            <span>重新启航</span>
          </button>
        </div>
      </div>

      {/* Footer Decoration */}
      <div className="absolute bottom-8 left-8 right-8 flex justify-between text-[10px] font-bold text-white/10 uppercase tracking-[0.5em] pointer-events-none">
        <span>Legend Of Uni: The Last Round</span>
        <span>Observation Post: Solar System</span>
      </div>
    </div>
  );
};

