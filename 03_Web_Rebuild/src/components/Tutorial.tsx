import React, { useState, useCallback } from 'react';
import { ChevronRight, X, Globe, Users, Zap, Shield, Rocket } from 'lucide-react';

interface TutorialStep {
  icon: React.ReactNode;
  title: string;
  description: string;
}

const TUTORIAL_STEPS: TutorialStep[] = [
  {
    icon: <Globe size={32} />,
    title: '欢迎来到宇宙群英传',
    description: '你将成为地球文明的最高指挥官，在三体危机中带领人类走向未知的宇宙纪元。\n\n面对异星文明的威胁，你需要平衡经济、军事、科技、外交——在黑暗森林中寻找生存之道。',
  },
  {
    icon: <Users size={32} />,
    title: '管理你的文明',
    description: '左侧导航栏是文明控制中心。\n\n点击星球查看详情并建设基地。在部门面板中任命合适的领导者，他们将极大提升部门的产出效率。',
  },
  {
    icon: <Zap size={32} />,
    title: '推进科技研究',
    description: '科技是文明进步的阶梯。\n\n部门会自动推荐并研究最低成本的可用科技。你也可以手动点击科技节点来优先研究关键项目。\n\n注意：智子封锁会大幅降低科研速度。',
  },
  {
    icon: <Shield size={32} />,
    title: '面壁计划与威慑',
    description: '在面壁者面板中任命最多 4 位面壁者，他们无需向外界解释自己的计划。\n\n设立执剑人建立黑暗森林威慑体系——当威慑值达到 100 时，你将获得对抗异星的终极武器。',
  },
  {
    icon: <Rocket size={32} />,
    title: '踏上征途',
    description: '点击右下角的「下一回合」推进时间线。\n\n每个回合：\n• 经济与人口自然增长\n• 建造设施逐步完工\n• 异星文明展开行动\n• 随机事件可能触发\n\n祝你好运，指挥官。',
  },
];

export const Tutorial: React.FC<{ onComplete: () => void }> = ({ onComplete }) => {
  const [step, setStep] = useState(0);
  const [exiting, setExiting] = useState(false);

  const handleNext = useCallback(() => {
    if (step < TUTORIAL_STEPS.length - 1) {
      setStep(s => s + 1);
    } else {
      setExiting(true);
      localStorage.setItem('game-tutorial-seen', 'true');
      setTimeout(onComplete, 400);
    }
  }, [step, onComplete]);

  const handleSkip = useCallback(() => {
    setExiting(true);
    localStorage.setItem('game-tutorial-seen', 'true');
    setTimeout(onComplete, 400);
  }, [onComplete]);

  const current = TUTORIAL_STEPS[step];

  return (
    <div className={`fixed inset-0 z-[500] flex items-center justify-center bg-black/80 backdrop-blur-sm transition-all duration-400 ${exiting ? 'opacity-0' : 'opacity-100'}`}>
      <div className={`w-full max-w-lg mx-4 glass-panel p-8 flex flex-col items-center text-center gap-6 transition-all duration-300 ${exiting ? 'scale-95 opacity-0' : 'scale-100 opacity-100'}`}>
        <button onClick={handleSkip} className="absolute top-4 right-4 text-[var(--text-secondary)] hover:text-[var(--text-primary)] transition-colors">
          <X size={20} />
        </button>

        <div className="w-16 h-16 rounded-full bg-[var(--color-primary-glass)] border border-[var(--color-primary)] flex items-center justify-center text-[var(--color-primary)]">
          {current.icon}
        </div>

        <div className="space-y-3">
          <h2 className="text-xl font-black text-[var(--text-primary)] tracking-tight">
            {current.title}
          </h2>
          <p className="text-sm text-[var(--text-secondary)] leading-relaxed whitespace-pre-line">
            {current.description}
          </p>
        </div>

        <div className="flex items-center gap-6 mt-4">
          <div className="flex gap-2">
            {TUTORIAL_STEPS.map((_, i) => (
              <div key={i} className={`w-2 h-2 rounded-full transition-colors ${i === step ? 'bg-[var(--color-primary)]' : i < step ? 'bg-[var(--color-primary)]/40' : 'bg-[var(--text-secondary)]/20'}`} />
            ))}
          </div>
          <button onClick={handleNext} className="flex items-center gap-2 px-6 py-2 bg-[var(--color-primary)] text-white dark:text-blue-950 font-bold rounded-lg hover:brightness-110 transition-all">
            {step < TUTORIAL_STEPS.length - 1 ? (
              <>下一步 <ChevronRight size={16} /></>
            ) : (
              '开始游戏'
            )}
          </button>
        </div>

        <button onClick={handleSkip} className="text-xs text-[var(--text-secondary)]/50 hover:text-[var(--text-secondary)] transition-colors mt-2">
          跳过教程
        </button>
      </div>
    </div>
  );
};