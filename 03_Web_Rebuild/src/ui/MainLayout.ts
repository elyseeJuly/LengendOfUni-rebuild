export class MainLayout {
  private container: HTMLElement;

  constructor(containerId: string) {
    const el = document.getElementById(containerId);
    if (!el) throw new Error(`Container #${containerId} not found`);
    this.container = el;
    this.render();
  }

  private render(): void {
    this.container.innerHTML = `
      <!-- Top Bar -->
      <div class="top-bar glass-panel">
        <div class="epoch-display" id="ui-epoch">危机纪元 0 年</div>
        <div class="resource-stats">
          <div class="stat-item">
            <span class="stat-label">人口</span>
            <span class="stat-value" id="ui-population">0</span>
          </div>
          <div class="stat-item">
            <span class="stat-label">经济</span>
            <span class="stat-value" id="ui-economy">0</span>
          </div>
          <div class="stat-item">
            <span class="stat-label">文化</span>
            <span class="stat-value" id="ui-culture">0</span>
          </div>
          <div class="stat-item">
            <span class="stat-label">军力</span>
            <span class="stat-value" id="ui-army">0</span>
          </div>
          <div class="stat-item">
            <span class="stat-label">威慑</span>
            <span class="stat-value" id="top-deterrence" style="color: #FFD700;">0</span>
          </div>
        </div>
        <div class="top-menu" style="display: flex; gap: 8px;">
          <button class="btn-glass" id="btn-history">历史记录</button>
          <button class="btn-glass" id="btn-system-menu" style="font-size: 1.2rem; padding: 4px 12px; border-radius: 6px;">⚙️</button>
        </div>
      </div>

      <!-- Star Map Area -->
      <div class="star-map-container" id="star-map-container">
        <canvas id="star-canvas"></canvas>
      </div>

      <!-- Right Panel -->
      <div class="right-panel glass-panel" id="right-panel">
        <div class="panel-header" id="panel-title">天体信息</div>
        <div id="panel-content" style="color: var(--text-secondary); font-size: 0.9rem;">
          请在左侧星图中选择天体以查看详情。
        </div>
      </div>

      <!-- Bottom Bar -->
      <div class="bottom-bar glass-panel">
        <div class="dept-controls" id="dept-buttons-container">
          <button class="btn-glass" data-dept="0">经济部</button>
          <button class="btn-glass" data-dept="1">军事部</button>
          <button class="btn-glass" data-dept="2">文化部</button>
          <button class="btn-glass" data-dept="3">人力资源部</button>
          <button class="btn-glass" data-dept="4">宇宙社会学</button>
          <button class="btn-glass" data-dept="5">核技术</button>
          <button class="btn-glass" data-dept="6">航天技术</button>
          <button class="btn-glass" data-dept="7">质子技术</button>
          <button class="btn-glass" data-dept="8">天体物理</button>
          <button class="btn-glass" data-dept="9">文化研究所</button>
          <button class="btn-glass" data-dept="10">经济研究所</button>
        </div>
        <button class="btn-primary" id="btn-next-turn">下一回合</button>
      </div>

      <!-- Modal Container -->
      <div id="modal-container" class="modal-overlay hidden">
        <div class="modal-box glass-panel">
          <div class="modal-header">
            <h2 id="modal-title">部门面板</h2>
            <button class="btn-close" id="btn-modal-close">&times;</button>
          </div>
          <div class="modal-content" id="modal-content"></div>
        </div>
      </div>
    `;
  }
}
