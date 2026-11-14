////////////////////////////////////////////
///                                      ///
///         LEGIONCORE REFORGED          ///
///    Copyright 2026 Titans-Project     ///
///         All Rights Reserved.         ///
///                                      ///
////////////////////////////////////////////


#ifndef GossipData_h
#define GossipData_h

enum class GossipOptionNpc : uint8;

struct GossipMenuItems
{
    ConditionList   Conditions;
    uint32          MenuID;
    uint32          OptionIndex;
    uint32          OptionBroadcastTextID;
    uint32          BoxBroadcastTextID;
    GossipOptionNpc OptionNpc;
    uint32          ActionMenuID;
    uint32          ActionPoiID;
    uint32          BoxMoney;
    std::string     OptionText;
    std::string     BoxText;
    bool            BoxCoded;
};

struct GossipMenus
{
    ConditionList   Conditions;
    uint32          MenuID;
    uint32          TextID;
};

typedef std::multimap<uint32, GossipMenus> GossipMenusContainer;
typedef std::pair<GossipMenusContainer::const_iterator, GossipMenusContainer::const_iterator> GossipMenusMapBounds;
typedef std::pair<GossipMenusContainer::iterator, GossipMenusContainer::iterator> GossipMenusMapBoundsNonConst;
typedef std::multimap<uint32, GossipMenuItems> GossipMenuItemsContainer;
typedef std::pair<GossipMenuItemsContainer::const_iterator, GossipMenuItemsContainer::const_iterator> GossipMenuItemsMapBounds;
typedef std::pair<GossipMenuItemsContainer::iterator, GossipMenuItemsContainer::iterator> GossipMenuItemsMapBoundsNonConst;
typedef std::unordered_map<uint64, GossipMenuItemsLocale> GossipMenuItemsLocaleContainer;

class TC_GAME_API GossipDataStoreMgr
{
    GossipDataStoreMgr();
    ~GossipDataStoreMgr();

public:
    static GossipDataStoreMgr* instance();

    void LoadGossipMenuItemsLocales();
    void LoadGossipMenu();
    void LoadGossipMenuItems();

    GossipMenuItemsLocale const* GetGossipMenuItemsLocale(uint64 entry) const;
    GossipMenusMapBounds GetGossipMenusMapBounds(uint32 uiMenuId) const;
    GossipMenusMapBoundsNonConst GetGossipMenusMapBoundsNonConst(uint32 uiMenuId);
    GossipMenuItemsMapBounds GetGossipMenuItemsMapBounds(uint32 uiMenuId) const;
    GossipMenuItemsMapBoundsNonConst GetGossipMenuItemsMapBoundsNonConst(uint32 uiMenuId);
    // the friendship reputation a menu shows (gossip_menu_addon), 0 for none
    int32 GetGossipMenuFriendshipFactionID(uint32 menuId) const;
private:
    std::unordered_map<uint32, int32> _gossipMenuFriendshipStore;
    GossipMenusContainer _gossipMenusStore;
    GossipMenuItemsContainer _gossipMenuItemsStore;
    GossipMenuItemsLocaleContainer _gossipMenuItemsLocaleStore;
};

#define sGossipDataStore GossipDataStoreMgr::instance()

#endif
