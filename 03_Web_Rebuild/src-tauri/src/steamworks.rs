/// Steamworks SDK 桥接 (预留接口)
///
/// 当前状态：禁用（Steam API 初始化仅在构建时启用）
/// 启用方式：在 Cargo.toml 中取消 steamworks 的注释
///
/// 功能：
/// - 成就解锁
/// - 统计数据追踪
/// - Steam 云存档

// use steamworks::{Client, SingleClient};

pub struct SteamBridge;

impl SteamBridge {
    pub fn new() -> Self {
        SteamBridge
    }

    pub fn is_available(&self) -> bool {
        false // Steam API 当前未启用
    }

    pub fn unlock_achievement(&self, _achievement_id: &str) -> bool {
        false
    }

    pub fn set_stat(&self, _stat_id: &str, _value: i32) -> bool {
        false
    }

    pub fn get_stat(&self, _stat_id: &str) -> i32 {
        0
    }
}

/// 成就定义常量
pub mod achievements {
    pub const ACH_FIRST_WALLFACER: &str = "ACH_FIRST_WALLFACER";
    pub const ACH_DETERRENCE_ESTABLISHED: &str = "ACH_DETERRENCE_ESTABLISHED";
    pub const ACH_WANDERING_EARTH: &str = "ACH_WANDERING_EARTH";
    pub const ACH_DIGITAL_SINGULARITY: &str = "ACH_DIGITAL_SINGULARITY";
    pub const ACH_GALAXY_ERA: &str = "ACH_GALAXY_ERA";
    pub const ACH_CONQUEST_VICTORY: &str = "ACH_CONQUEST_VICTORY";
    pub const ACH_SURVIVAL_500: &str = "ACH_SURVIVAL_500";
    pub const ACH_ALL_TECHS: &str = "ACH_ALL_TECHS";
}