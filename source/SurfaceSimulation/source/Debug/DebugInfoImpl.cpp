//#include "stdafx.h"
//#include "DebugInfoImpl.h"
//#include "math\math_utils.h"
//#include "colreg/BaseRefHolder.h"
//
//namespace dbg
//{
//   class DebugInfoNode;
//   using DebugInfoNodes = std::vector<DebugInfoNode>;
//
//   class DebugInfoNode
//   {
//   public:
//      DebugInfoNode(const std::wstring& name = {})
//         : m_name{ name }
//      {}
//
//      const std::wstring& Name() const { return m_name; }
//
//      DebugInfoNodes& GetSubnodes() { return m_subnodes; }
//
//      const dbg::debug_info_node* PrepareRef()
//      {
//         m_ref = { m_name.c_str(),
//                  colreg::geo_points_vct{ m_points.data(), m_points.size() },
//                  prepareSubnodesRef(),
//                  dbg::debug_drawing_info{ m_text.empty() ? nullptr : m_text.c_str(), m_pixSz, m_color, m_ptsLinkage } };
//         return &m_ref;
//      }
//
//      void SetData(dbg::dbg_points_vct&& pts,
//         const std::string& text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage)
//      {
//         m_points = std::move(pts);
//         m_text = text;
//         m_pixSz = pixSz;
//         m_color = color;
//         m_ptsLinkage = ptsLinkage;
//      }
//
//      DebugInfoNode* AddSubnode(const std::wstring& name = {})
//      {
//         m_subnodes.push_back(name);
//         return &m_subnodes.back();
//      }
//
//      void Clear()
//      {
//         m_subnodes.clear();
//         m_subnodesRef.clear();
//         m_points.clear();
//
//         m_name = {};
//         m_text = {};
//         m_pixSz = dbg::DEFAULT_PIX_SIZE;
//         m_color = dbg::DEFAULT_COLOR;
//         m_ptsLinkage = dbg::DEFAULT_LINKAGE;
//
//         m_ref = {};
//      }
//
//   private:
//      inline dbg::debug_info_node_vct_ref prepareSubnodesRef()
//      {
//         m_subnodesRef.clear();
//         m_subnodesRef.reserve(m_subnodes.size());
//         for (DebugInfoNode& node : m_subnodes)
//            m_subnodesRef.push_back(*node.PrepareRef());
//         return { m_subnodesRef.data(), m_subnodesRef.size() };
//      }
//
//   private:
//      std::wstring m_name;
//
//      dbg::dbg_points_vct m_points;
//
//      std::string m_text = {};
//      size_t m_pixSz = dbg::DEFAULT_PIX_SIZE;
//      size_t m_color = dbg::DEFAULT_COLOR;
//      dbg::PointsLinkage m_ptsLinkage = dbg::DEFAULT_LINKAGE;
//
//      DebugInfoNodes m_subnodes;
//
//      dbg::debug_info_node m_ref;
//      std::vector<dbg::debug_info_node> m_subnodesRef;
//   };
//
//   struct iDebugInfoHolder
//   {
//      virtual const dbg::debug_info_node_roots_ref* GetRoots() const = 0;
//      virtual void AddSubnode(const dbg::dbg_info_node_path_vct& nodePath, dbg::dbg_points_vct&& pts,
//         const std::string& text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage) = 0;
//      virtual void ClearNode(const dbg::dbg_info_node_path_vct& nodePath) = 0;
//      virtual void PrepareRef() = 0;
//      virtual void Clear() = 0;
//      virtual void Release() { delete this; }
//   protected:
//      virtual ~iDebugInfoHolder() = default;
//   };
//
//   class DebugInfoHolder : public iDebugInfoHolder
//   {
//   public:
//      const dbg::debug_info_node_roots_ref* GetRoots() const override { return nullptr; }
//      const dbg::debug_info_node* GetRoot() const { return _rootRef; }
//
//      //! Äîáàâëÿåò òî÷êè êàê ïîäýëåìåíò äëÿ nodePath
//      void AddSubnode(const dbg::dbg_info_node_path_vct& nodePath, dbg::dbg_points_vct&& pts,
//         const std::string& text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage) override
//      {
//         getNode(nodePath)->AddSubnode()->SetData(std::move(pts), text, pixSz, color, ptsLinkage);
//         _needUpdateRef = true;
//      }
//
//      void ClearNode(const dbg::dbg_info_node_path_vct& nodePath) override
//      {
//         clearNode(nodePath);
//      }
//
//      void PrepareRef() override
//      {
//         if (_needUpdateRef)
//            _rootRef = _root.PrepareRef();
//         _needUpdateRef = false;
//      }
//
//      void Clear() override { _root.Clear(); }
//
//   private:
//      ~DebugInfoHolder() override = default;
//
//      void clearNode(const dbg::dbg_info_node_path_vct& nodePath)
//      {
//         if (nodePath.empty())
//            return;
//         DebugInfoNode* node = &_root;
//         DebugInfoNodes* subnodes = &_root.GetSubnodes();
//         DebugInfoNodes::iterator nodeIt = subnodes->end();
//         for (const std::wstring& nodeName : nodePath)
//         {
//            nodeIt = findNode(nodeName, subnodes);
//            if (nodeIt == subnodes->end())
//            {
//               return; //íå íàøëè êóñîê ïóòè-çíà÷èò åãî íåò
//            }
//
//            DebugInfoNode* subnode = &*nodeIt;
//            subnodes = &subnode->GetSubnodes();
//            node = subnode;
//         }
//
//         subnodes->clear();
//      }
//
//      DebugInfoNode* getNode(const dbg::dbg_info_node_path_vct& nodePath)
//      {
//         if (nodePath.empty())
//            return &_root;
//
//         DebugInfoNode* node = &_root;
//         DebugInfoNodes* subnodes = &_root.GetSubnodes();
//         for (const std::wstring& nodeName : nodePath)
//         {
//            auto nodeIt = findNode(nodeName, subnodes);
//            DebugInfoNode* subnode = (nodeIt == subnodes->end() ? nullptr : &*nodeIt);
//            if (subnode == nullptr)
//               subnode = node->AddSubnode(nodeName);
//            subnodes = &subnode->GetSubnodes();
//            node = subnode;
//         }
//         return node;
//      }
//
//      DebugInfoNodes::iterator findNode(const std::wstring& nodeName, DebugInfoNodes* nodes)
//      {
//         ATLASSERT(nodes);
//         std::wstring lowerNameToFind = nodeName;
//         std::transform(lowerNameToFind.begin(), lowerNameToFind.end(), lowerNameToFind.begin(), tolower);
//         auto nodeIt = find_if(nodes->begin(), nodes->end(), [&lowerNameToFind](const DebugInfoNode& node)
//            {
//               std::wstring lowerName = node.Name();
//               std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), tolower);
//               return lowerName == lowerNameToFind;
//            });
//         return nodeIt;
//      }
//
//   private:
//      bool _needUpdateRef = true;
//
//      DebugInfoNode _root;
//      const dbg::debug_info_node* _rootRef;
//   };
//
//   //! êëàññ äëÿ ðàáîòû ñ íåñêîëüêèìè DebugInfoHolder
//   class DebugInfoHolderManager : public iDebugInfoHolder
//   {
//   public:
//      const dbg::debug_info_node_roots_ref* GetRoots() const override { return &_roots; }
//
//      //! Äîáàâëÿåò òî÷êè êàê ïîäýëåìåíò äëÿ nodePath
//      void AddSubnode(const dbg::dbg_info_node_path_vct& nodePath, dbg::dbg_points_vct&& pts,
//         const std::string& text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage) override
//      {
//         _debugInfoHolders.front().debugHolder->AddSubnode(nodePath, std::move(pts), text, pixSz, color, ptsLinkage);
//      }
//
//      void PrepareRef() override
//      {
//         std::for_each(_debugInfoHolders.begin(), _debugInfoHolders.end(), [](auto& dbgHolder) { dbgHolder.debugHolder->PrepareRef(); });
//      }
//
//      void Clear() { std::for_each(_debugInfoHolders.begin(), _debugInfoHolders.end(), [](auto& dbgHolder) { dbgHolder.debugHolder->Clear(); }); }
//      void Clear(const char* name)
//      {
//         std::for_each(_debugInfoHolders.begin(), _debugInfoHolders.end(), [name](auto& dbgHolder)
//            { if (!name || _stricmp(name, dbgHolder.name.c_str()) == 0) dbgHolder.debugHolder->Clear();
//            });
//      }
//      void ClearNode(const dbg::dbg_info_node_path_vct& nodePath) override
//      {
//         std::for_each(_debugInfoHolders.begin(), _debugInfoHolders.end(), [&nodePath](auto& dbgHolder) { dbgHolder.debugHolder->ClearNode(nodePath); });
//      }
//      DebugInfoHolderManager()
//      {
//         add("", new DebugInfoHolder());
//      }
//      void AddExternalDebugInfoHolder(const char* name, void* externalDebugInfoholder)
//      {
//         add(name, (DebugInfoHolder*)externalDebugInfoholder);
//      }
//
//   protected:
//      void add(const char* name, DebugInfoHolder* holder)
//      {
//         module_debug_info di;
//         di.name = name ? name : "";
//         di.debugHolder = holder;
//         _debugInfoHolders.push_back(di);
//         _debugInfoHolders.back().debugHolder->PrepareRef();
//         _roots.Data().push_back(_debugInfoHolders.back().debugHolder->GetRoot());
//         _roots.Rebase();
//      }
//   public:
//      ~DebugInfoHolderManager() override
//      {
//         _debugInfoHolders.front().debugHolder->Release();//destroy only own
//         _debugInfoHolders.clear();
//      }
//   protected:
//      struct module_debug_info
//      {
//         std::string name;
//         DebugInfoHolder* debugHolder;
//      };
//      std::vector < module_debug_info> _debugInfoHolders; //ïåðâûé - ñîáñòâåííûé - îñòàëüíûå âíåøíèå
//      BaseRefHolder<const dbg::debug_info_node*> _roots;
//   };
//
//   class DebugInfoImpl
//      : public dbg::iDebugInfo
//   {
//   public:
//      void Enable(colreg::id_type id, bool enable) override { _enabled[id] = enable; };
//      bool IsEnabled(colreg::id_type id) const override { return _enabled.find(id) != _enabled.end() ? _enabled.at(id) : false; };
//      const dbg::debug_info_node_roots_ref* GetNodes() const override { return _manager.GetRoots(); };
//      DebugInfoHolderManager& GetManager() { return _manager; }
//      void AddPoints(colreg::id_type id, const dbg::dbg_info_node_path_ref& nodePath, const dbg::dbg_points_ref& pts, const char* text, size_t pixSz, size_t color) override
//      {
//         ATLASSERT(IsEnabled(id));
//         BaseRefHolder<const wchar_t*> path(&nodePath);
//         AddShipLevel(id, charRefToVct(path));
//         AddPoints(path, pts, text, pixSz, color);
//      }
//
//      void AddPoints(const dbg::dbg_info_node_path_ref& nodePath, const dbg::dbg_points_ref& pts, const char* text, size_t pixSz, size_t color) override
//      {
//         BaseRefHolder<const wchar_t*> path(&nodePath);
//         BaseRefHolder<dbg::dbg_point> points(&pts);
//         GetManager().AddSubnode(charRefToVct(path), std::move(points.Data()), text, pixSz, color, dbg::PointsLinkage::PL_NONE);
//      }
//
//      void AddLine(colreg::id_type id, const dbg::dbg_info_node_path_ref& nodePath, const dbg::dbg_points_ref& pts, const char* text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage) override
//      {
//         ATLASSERT(IsEnabled(id));
//         BaseRefHolder<const wchar_t*> path(&nodePath);
//         AddShipLevel(id, charRefToVct(path));
//         AddLine(path, pts, text, pixSz, color, ptsLinkage);
//      }
//
//      void AddLine(const dbg::dbg_info_node_path_ref& nodePath, const dbg::dbg_points_ref& pts, const char* text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage) override
//      {
//         BaseRefHolder<const wchar_t*> path(&nodePath);
//         BaseRefHolder<dbg::dbg_point> points(&pts);
//
//         GetManager().AddSubnode(charRefToVct(path), std::move(points.Data()), text, pixSz, color, ptsLinkage);
//      }
//
//      void AddArc(colreg::id_type id, dbg::dbg_info_node_path_ref& nodePath, const dbg::dbg_point& center, double radius,
//         double beg, double end, double step,
//         const char* text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage) override
//      {
//         ATLASSERT(IsEnabled(id));
//         BaseRefHolder<const wchar_t*> path(&nodePath);
//         AddShipLevel(id, charRefToVct(path));
//         AddArc(path, center, radius, beg, end, step, text, pixSz, color, ptsLinkage);
//      }
//
//      void AddArc(const dbg::dbg_info_node_path_ref& nodePath, const dbg::dbg_point& center, double radius,
//         double beg, double end, double step,
//         const char* text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage) override
//      {
//         BaseRefHolder<const wchar_t*> path(&nodePath);
//         dbg::dbg_points_vct pts;
//         pts.reserve(static_cast<size_t>((end - beg) / step));
//         for (double deg = beg; deg <= end; deg += step)
//            pts.emplace_back(math::calc_point(center, radius, deg));
//
//         GetManager().AddSubnode(charRefToVct(path), std::move(pts), text, pixSz, color, ptsLinkage);
//      }
//      void AddShipLevel(colreg::id_type id, dbg::dbg_info_node_path_vct& nodePath)
//      {
//         std::wstring msg = L"ship_" + std::to_wstring(id);
//         nodePath.insert(nodePath.begin(), msg);
//      }
//   protected:
//      dbg::dbg_info_node_path_vct charRefToVct(BaseRefHolder<const wchar_t*>& ref)
//      {
//         dbg::dbg_info_node_path_vct msgs_s;
//         const std::vector<const wchar_t*> msgs_c = ref.Data();
//         std::transform(msgs_c.begin(), msgs_c.end(), std::back_inserter(msgs_s), [](const wchar_t* str) -> std::wstring { return std::wstring(str); });
//         return msgs_s;
//      }
//   private:
//      DebugInfoHolderManager _manager;
//      std::unordered_map<colreg::id_type, bool> _enabled;
//   };
//   DebugInfoImpl* g_debugInfo = nullptr;
//}
//
//namespace dbg
//{
//   iDebugInfo* CreateDebugInfoManager()
//   {
//      if (!g_debugInfo)
//         g_debugInfo = new DebugInfoImpl{};
//      g_debugInfo->GetManager().PrepareRef();
//      return reinterpret_cast<iDebugInfo*>(g_debugInfo);
//   }
//
//   iDebugInfo* GetDebugInfoManager()
//   {
//      return reinterpret_cast<iDebugInfo*>(g_debugInfo);
//   }
//
//   void DestroyDebugger()
//   {
//      if (!g_debugInfo)
//         return;
//      delete g_debugInfo;
//      g_debugInfo = nullptr;
//   }
//
//   bool IsDebuggingEnabled()
//   {
//      //return false;//TODO îòêëþ÷èë äî ïåðåäåëûâàíèÿ äåáàæíîãî èíòåðôåéñà
//      return g_debugInfo != nullptr;
//   }
//
//   void ClearDebugNode(const dbg_info_node_path_vct& nodePath)
//   {
//      if (g_debugInfo)
//         g_debugInfo->GetManager().ClearNode(nodePath);
//   }
//
//   void ClearDebugInfo(const char* moduleName)
//   {
//      if (!g_debugInfo)
//         return;
//      auto manager = dynamic_cast<DebugInfoHolderManager*>(g_debugInfo);
//
//      if (!moduleName || !manager)
//      {
//         //clear all
//         g_debugInfo->GetManager().Clear();
//      }
//      else
//         manager->Clear(moduleName);
//   }
//
//   bool IsDebuggingEnabled(colreg::id_type id) { return IsDebuggingEnabled() && g_debugInfo->IsEnabled(id); }
//
//   void* CreateDebugInfoHolder()
//   {
//      if (!g_debugInfo)
//         g_debugInfo = new DebugInfoImpl{};
//      g_debugInfo->GetManager().PrepareRef();
//      return reinterpret_cast<void*>(g_debugInfo);
//   }
//
//   bool AddExternalDebugInfo(const char* name, void* extDebugger)
//   {
//      if (!extDebugger || !g_debugInfo)
//         return false;
//
//      auto dbgInfoHolder = dynamic_cast<DebugInfoHolder*>((iDebugInfoHolder*)extDebugger);
//      if (!dbgInfoHolder)
//         return false;
//
//      auto manager = dynamic_cast<DebugInfoHolderManager*>(g_debugInfo);
//      if (!manager)
//         return false;
//
//      manager->AddExternalDebugInfoHolder(name, dbgInfoHolder);
//      return true;
//   }
//
//   void AddPoints(colreg::id_type id, dbg_info_node_path_vct nodePath, dbg_points_vct&& pts, const std::string& text, size_t pixSz, size_t color)
//   {
//      if (g_debugInfo)
//      {
//         ATLASSERT(g_debugInfo->IsEnabled(id));
//         g_debugInfo->AddShipLevel(id, nodePath);
//         AddPoints(nodePath, std::move(pts), text, pixSz, color);
//      }
//   }
//
//   void AddPoints(const dbg_info_node_path_vct& nodePath, dbg_points_vct&& pts, const std::string& text, size_t pixSz, size_t color)
//   {
//      if (g_debugInfo)
//         g_debugInfo->GetManager().AddSubnode(nodePath, std::move(pts), text, pixSz, color, PointsLinkage::PL_NONE);
//   }
//
//   void AddLine(colreg::id_type id, dbg_info_node_path_vct nodePath, dbg_points_vct&& pts, const std::string& text, size_t pixSz, size_t color, PointsLinkage ptsLinkage)
//   {
//      if (g_debugInfo)
//      {
//         ATLASSERT(g_debugInfo->IsEnabled(id));
//         g_debugInfo->AddShipLevel(id, nodePath);
//         AddLine(nodePath, std::move(pts), text, pixSz, color, ptsLinkage);
//      }
//   }
//
//   void AddLine(const dbg_info_node_path_vct& nodePath, dbg_points_vct&& pts, const std::string& text, size_t pixSz, size_t color, PointsLinkage ptsLinkage)
//   {
//      if (g_debugInfo)
//         g_debugInfo->GetManager().AddSubnode(nodePath, std::move(pts), text, pixSz, color, ptsLinkage);
//   }
//
//   void AddArc(colreg::id_type id, dbg_info_node_path_vct nodePath, const dbg_point& center, double radius,
//      double beg, double end, double step,
//      const std::string& text, size_t pixSz, size_t color, PointsLinkage ptsLinkage)
//   {
//      if (g_debugInfo)
//      {
//         ATLASSERT(g_debugInfo->IsEnabled(id));
//         g_debugInfo->AddShipLevel(id, nodePath);
//         AddArc(nodePath, center, radius, beg, end, step, text, pixSz, color, ptsLinkage);
//      }
//   }
//
//   void AddArc(const dbg::dbg_info_node_path_vct& nodePath, const dbg::dbg_point& center, double radius,
//      double beg, double end, double step,
//      const std::string& text, size_t pixSz, size_t color, dbg::PointsLinkage ptsLinkage)
//   {
//      step *= math::sign(end - beg);
//      dbg_points_vct pts;
//      pts.reserve(static_cast<size_t>((end - beg) / step));
//      for (double deg = beg; deg <= end; deg += step)
//         pts.emplace_back(math::calc_point(center, radius, deg));
//
//      if (g_debugInfo)
//         g_debugInfo->GetManager().AddSubnode(nodePath, std::move(pts), text, pixSz, color, ptsLinkage);
//   }
//}