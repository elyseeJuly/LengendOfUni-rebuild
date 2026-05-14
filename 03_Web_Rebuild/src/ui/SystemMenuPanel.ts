import { GameInstance } from "../core/Game";

export class SystemMenuPanel {
  private container: HTMLElement;
  private mounted: boolean = false;

  constructor() {
    this.container = document.createElement("div");
    this.container.id = "system-menu-modal";
    this.container.className = "modal-overlay hidden";
    this.container.style.zIndex = "3000"; 
    
    this.container.innerHTML = `
      <div class="modal-box glass-panel" style="max-width: 400px; height: auto; padding: 24px;">
        <div class="modal-header" style="border-bottom: none; padding: 0 0 16px 0; justify-content: center;">
          <h2 style="font-size: 1.8rem; letter-spacing: 2px;">系统菜单</h2>
        </div>
        <div class="modal-content" style="padding: 0; display: flex; flex-direction: column; gap: 16px;">
          <button class="btn-primary" id="btn-save-game" style="width: 100%;">💾 保存游戏</button>
          <button class="btn-glass" id="btn-load-game" style="width: 100%; font-size: 1.1rem; padding: 12px;">📂 读取游戏</button>
          <button class="btn-glass" id="btn-toggle-theme" style="width: 100%; font-size: 1.1rem; padding: 12px;">🌓 切换主题 (明/暗)</button>
          <button class="btn-glass" id="btn-restart" style="width: 100%; font-size: 1.1rem; padding: 12px; border-color: #FF5500; color: #FF5500;">🔄 重新开始</button>
          <div style="margin-top: 16px; padding-top: 16px; border-top: 1px solid rgba(255,255,255,0.1); text-align: center;">
            <button class="btn-glass" id="btn-close-menu" style="width: 100%;">返回游戏</button>
          </div>
        </div>
      </div>
    `;
    
    this.bindEvents();
  }

  public open() {
    if (!this.mounted) {
      document.body.appendChild(this.container);
      this.mounted = true;
    }
    this.container.classList.remove("hidden");
  }

  public close() {
    this.container.classList.add("hidden");
  }

  private bindEvents() {
    this.container.querySelector("#btn-close-menu")?.addEventListener("click", () => this.close());
    
    this.container.querySelector("#btn-save-game")?.addEventListener("click", () => {
      GameInstance.saveGame();
      alert("游戏保存成功！");
      this.close();
    });

    this.container.querySelector("#btn-load-game")?.addEventListener("click", () => {
      if (confirm("读取游戏将丢失当前未保存的进度，确认读取？")) {
        const success = GameInstance.loadGame();
        if (success) {
          alert("游戏读取成功！");
          this.close();
          window.dispatchEvent(new Event("game-loaded"));
        } else {
          alert("没有找到存档数据！");
        }
      }
    });

    this.container.querySelector("#btn-toggle-theme")?.addEventListener("click", () => {
      document.documentElement.classList.toggle("dark");
      console.log("Theme toggled: ", document.documentElement.classList.contains("dark") ? "Dark" : "Light");
    });

    this.container.querySelector("#btn-restart")?.addEventListener("click", () => {
      if (confirm("确认放弃当前进度重新开始吗？")) {
        GameInstance.reset();
        window.location.reload();
      }
    });
  }
}

export const systemMenuPanel = new SystemMenuPanel();
