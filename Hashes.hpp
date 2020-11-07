namespace Mystic::Hashes {

	inline int vehicle_super_index = 0;
	inline int vehicle_sports_index = 0;
	inline int vehicle_sports_classic_index = 0;
	inline int vehicle_sedan_index = 0;
	inline int vehicle_motorcycle_index = 0;
	inline int vehicle_boat_index = 0;
	inline int vehicle_commercial_index = 0;
	inline int vehicle_compact_index = 0;
	inline int vehicle_coupe_index = 0;
	inline int vehicle_cycle_index = 0;
	inline int vehicle_emergency_index = 0;
	inline int vehicle_helicopter_index = 0;
	inline int vehicle_industrial_index = 0;
	inline int vehicle_military_index = 0;
	inline int vehicle_muscle_index = 0;
	inline int vehicle_offroad_index = 0;
	inline int vehicle_plane_index = 0;
	inline int vehicle_service_index = 0;
	inline int vehicle_suv_index = 0;
	inline int vehicle_trailer_index = 0;
	inline int vehicle_train_index = 0;
	inline int vehicle_utility_index = 0;
	inline int vehicle_van_index = 0;

	inline int vehicle_casinoheist_index = 0;
	inline int vehicle_casino_index = 0;
	inline int vehicle_arenawars_index = 0;
	inline int vehicle_afterhours_index = 0;
	inline int vehicle_ssas_index = 0;
	inline int vehicle_doomsday_index = 0;
	inline int vehicle_smugglers_index = 0;
	inline int vehicle_gunrunning_index = 0;
	inline int vehicle_cunningstunts_index = 0;
	inline int vehicle_importexport_index = 0;
	
	const DWORD forbidden_object[] =
	{
		0xdf9841d7, 0xceea3f4b, 0x2592b5cf, 0x9cf21e0f, 0xc1ce1183, 0x49863e9c, 0xcd93a7db, 0x82cac433, 0x74f24de,  0x1c725a1,  0x81fb3ff0,
		0x79b41171, 0x781e451d, 0xa5e3d471, 0x6a27feb1, 0x861d914d, 0x8c049d17, 0xffba70aa, 0xe65ec0e4, 0xc3c00861, 0x5f5dd65c,
		0xc07792d4, 0x53cfe80a, 0xd9f4474c, 0xcb2acc8,  0xc6899cde, 0xd14b5ba3, 0x32a9996c, 0x69d4f974, 0xc2e75a21, 0x1075651,
		0xe1aeb708, 0xcbbb6c39, 0x6fa03a5e, 0xcf7a9a9d, 0x34315488, 0x45ef7804, 0xac3de147, 0xcafc1ec3, 0xd971bbae, 0xe764d794,
		0xf51f7309, 0x1e78c9d,  0xa49658fc, 0x4f2526da, 0x576ab4f6, 0xd20b1778, 0x54bc1cd8, 0xce109c5c, 0xe049c0ce, 0x78de93d1,
		0xe5b89d31, 0x5850e7b3, 0x6aed0e4b, 0xc50a4285, 0xb648a502, 0x5e497511, 0x47c14801, 0xfd8bb397, 0xef541728, 0xc2cc99d8,
		0x8fb233a4, 0x24e08e1f, 0x337b2b54, 0x7367d224, 0x919d9255, 0x4484243f, 0x3c91d42d, 0x3353525a, 0xc175f658, 0x762657c6,
		0x94ac15b3, 0x28014a56, 0xe0a8bfc9, 0x3a559c31, 0x5fc8a70,  0x3b545487, 0xb9402f87
	};

	inline const char* vehicle_super[31]
	{
		"Truffade Adder", "Bravado Banshee 900R", "Vapid Bullet", "Cheetah", "Entity XF",
		"Vapid  FMJ", "Progen GP1", "Emperor ETR 1", "Infernus", "Pegassi Osiris",
		"Annis RE-7B", "Pfister 811", "X80 Proto", "Reaper", "Sultan RS",
		"Progen T20", "Turismo R", "Tyrus", "Vacca", "Voltic",
		"Zentorno", "Overflod  Tyrant", "Pegassi Tezeract", "Cheval Taipan", "Entity XXR",
		"Overflod  Autarch", "Comet SR", "Ubermacht SC 1", "Coil Cyclone", "Grotti Visione",
		"Ocelot XA 21"
	};

	inline std::vector<Hash> vehicle_super_hash
	{
		0xb779a091, 0x25c5af13, 0x9ae6dda1, 0xb1d95da0, 0xb2fe5cf9,	//"ADDER", "BANSHEE2", "BULLET", "CHEETAH", "ENTITYXF",
		0x5502626c, 0x4992196c, 0x30d3f6d8, 0x18f25ac7, 0x767164d6,	//"FMJ", "GP1", "SHEAVA", "INFERNUS", "OSIRIS",
		0xb6846a55, 0x92ef6e04, 0x7e8f677f, 0xdf381e5, 0xee6024bc,	//"LE7B", "PFISTER811", "PROTOTIPO", "REAPER", "SULTANRS",
		0x6322b39a, 0x185484e1, 0x7b406efb, 0x142e0dc3, 0x9f4b77be,	//"T20", "TURISMOR", "TYRUS", "VACCA", "VOLTIC",
		0xac5df515,	0xE99011C2, 0x3D7C6410, 0xBC5DC07E, 0x8198AEDC,	//"ZENTORNO", "TYRANT", "TEZERACT", "TAIPAN", "ENTITY XXR" 
		0xED552C74, 0x5097F589, 0x52FF9437, 0x59A9E570, 0x7397224C,  // AUTARCH COMMET SR SC 1 COIL CYCLONE VISIONE
		0x36B4A8A9 // XA21
	};

	inline const char* vehicle_sports[45]
	{
		"Albany Alpha", "Bravado Banshee", "Grotti Bestia GTS", "Dinka Blista Compact", "Dinka Blista Go GO Monkey",
		"Bravado Buffalo", "Bravado Buffalo S", "Bravado Buffalo Spunk", "Grotti Carbonizzare", "Pfister Comet",
		"Invetero Coquette", "Annis Elegy RH8", "Benefactor Feltzer", "Lampadati Furore GT", "Schyster Fusilade",
		"Karin Futo", "Dinka Jester", "Dinka Jester (Racecar)", "Hyak Khamelion", "Karin Kuruma",
		"Karin Kuruma (Armored)", "Ocelot Lynx", "Dewbauchee Massacro", "Massacro (Racecar)", "Obey 9F",
		"Obey 9F Cabrio", "Obey Omnis", "Maibatsu Penumbra", "Dewbauchee Rapid GT", "Dewbauchee Rapid GT 2",
		"BF Raptor", "Hyak Ruston", "Benefactor Schafter V12", "Benefactor Schafter LWB", "Schwartzer", // No Schwartzer
		"Dewbauchee Seven-70", "Karin Sultan", "Benefactor Surano", "Declasse Drift Tampa", "Lampadati Tropos Rallye",
		"Bravado Verlierer", "Vapid Flash GT", "Vapid GB200", "Declasse Hotring Sabre", "Pfister Comet SR"
	};

	inline std::vector<Hash> vehicle_sports_hash
	{
		0x2db8d1aa, 0xc1e908d2, 0x4bfcf28b, 0x3dee5eda, 0xdcbc1c3b,	//"ALPHA", "BANSHEE", "BESTIAGTS", "BLISTA2", "BLISTA3",
		0xedd516c6, 0x2bec3cbe, 0xe2c013e, 0x7b8ab45f, 0xc1ae4d16,	//"BUFFALO", "BUFFALO2", "BUFFALO3", "CARBONIZZARE", "COMET2",
		0x67bc037, 0xde3d9d22, 0x8911b9f5, 0xbf1691e0, 0x1dc0ba53,	//"COQUETTE", "ELEGY2", "FELTZER2", "FUROREGT", "FUSILADE",
		0x7836ce2f, 0xb2a716a3, 0xbe0e6126, 0x206d1b68, 0xae2bfe94,	//"FUTO", "JESTER", "JESTER2", "KHAMELION", "KURUMA",
		0x187d938d, 0x1cbdc10b, 0xf77ade32, 0xda5819a3, 0x3d8fa25c,	//"KURUMA2", "LYNX", "MASSACRO", "MASSACRO2", "NINEF",
		0xa8e38b01, 0xd1ad4937, 0xe9805550, 0x8cb29a14, 0x679450af,	//"NINEF2", "OMNIS", "PENUMBRA", "RAPIDGT", "RAPIDGT2",
		0xd7c56d39, 0x2ae524a8, 0xa774b5a6, 0x58cf185c, 0x2b122c82,	//"RAPTOR", "RUSTON", "SCHAFTER3", "SCHAFTER4", "SCHWARTZER",
		0x97398a4b, 0x39da2754, 0x16e478c1, 0xc0240885, 0x707e63a4,	//"SEVEN70", "SULTAN", "SURANO", "TAMPA2", "TROPOS",
		0x41b77fa4,	0xB4F32118, 0x71CBEA98, 0x42836BE5, 0x276D98A3	//"VERLIERER2", "FLASHGT", "GB200", "HOTRING", "COMET5"
	};

	inline const char* vehicle_sports_classic[32]
	{
		"Albany Roosevelt", "Albany Franken Stange", "Albany Roosevelt Valor", "Lampadati Casco", "Invetero Coquette Classic",
		"Benefactor Stirling GT", "Pegassi Infernus Classic", "Dewbauchee JB 700", "Declasse Mamba", "Albany Manana",
		"Pegassi Monroe", "Vapid Peyote", "Lampadati Pigalle", "Grotti Stinger", "Grotti Stinger GT",
		"Declasse Tornado", "Declasse Tornado 2", "Declasse Tornado 3", "Declasse Tornado 4", "Declasse Tornado Custom",
		"Declasse Tornado Rat Rod", "Grotti Turismo Classic", "Truffade Z-Type", "CHEBUREK", "Vulcar Fagaloa", "Dinka Jester Classic", // Mali Cheburek, Torero, Vagner, XA-21, Free crawler
		"Lampadati Michelli GT", "Grotti Cheetah Classic", "Torero", "Vagner", "XA 21", "Free Crawler"
	};

	inline std::vector<Hash> vehicle_sports_classic_hash
	{
		0x6ff6914, 0xce6b35a4, 0xdc19d101, 0x3822bdfe, 0x3c4e2113,	//"BTYPE", "BTYPE2", "BTYPE3", "CASCO", "COQUETTE2",
		0xa29d6d10, 0xac33179c, 0x3eab5555, 0x9cfffc56, 0x81634188,	//"FELTZER3", "INFERNUS2", "JB700", "MAMBA", "MANANA",
		0xe62b361b, 0x6d19ccbc, 0x404b6381, 0x5c23af9b, 0x82e499fa,	//"MONROE", "PEYOTE", "PIGALLE", "STINGER", "STINGERGT",
		0x1bb290bc, 0x5b42a5c4, 0x690a4153, 0x86cf7cdd, 0x94da98ef,	//"TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TORNADO5",
		0xa31cb573, 0xc575df11, 0x2d3bd401,	0x4ABEBF23, 0x6068AD86,	//"TORNADO6", "TURISMO2", "ZTYPE", "CHEBUREK", "FAGALOA"
		0xF330CB6A, 0x3E5BD8D9, 0x0D4E5F4D, 0xFCC2F483     // "JESTER3", "MICHELLI"
	};

	inline const char* vehicle_sedan[32]
	{
		"Asea", "Asea 2", "Asterope", "Cognoscenti 55", "Cognoscenti 552",
		"Cognoscenti", "Cognoscenti 2", "Emperor ", "Emperor 2", "Emperor 3",
		"FUGITIVE", "GLENDALE", "INGOT", "INTRUDER", "LIMO2",
		"PREMIER", "PRIMO", "PRIMO2", "REGINA", "ROMERO",
		"SCHAFTER2", "SCHAFTER5", "SCHAFTER6", "STANIER", "STRATUM",
		"STRETCH", "SUPERD", "SURGE", "TAILGATER", "WARRENER",
		"WASHINGTON", "Stafford"
	};

	inline std::vector<Hash> vehicle_sedan_hash
	{
		0x94204d89, 0x9441d8d5, 0x8e9254fb, 0x360a438e, 0x29fcd3e4,	//"ASEA", "ASEA2", "ASTEROPE", "COG55", "COG552",
		0x86fe0b60, 0xdbf2d57a, 0xd7278283, 0x8fc3aadc, 0xb5fcf74e,	//"COGNOSCENTI", "COGNOSCENTI2", "EMPEROR", "EMPEROR2", "EMPEROR3",
		0x71cb2ffb, 0x47a6bc1, 0xb3206692, 0x34dd8aa1, 0xf92aec4d,	//"FUGITIVE", "GLENDALE", "INGOT", "INTRUDER", "LIMO2",
		0x8fb66f9b, 0xbb6b404f, 0x86618eda, 0xff22d208, 0x2560b2fc,	//"PREMIER", "PRIMO", "PRIMO2", "REGINA", "ROMERO",
		0xb52b5113, 0xcb0e7cd9, 0x72934be4, 0xa7ede74d, 0x66b4fc45,	//"SCHAFTER2", "SCHAFTER5", "SCHAFTER6", "STANIER", "STRATUM",
		0x8b13f083, 0x42f2ed16, 0x8f0e3594, 0xc3ddfdce, 0x51d83328,	//"STRETCH", "SUPERD", "SURGE", "TAILGATER", "WARRENER",
		0x69f06b57,	0x1324E960 									//"WASHINGTON"
	};

	inline const char* vehicle_motorcycle[43]
	{
		"Dinka Akuma", "LCC Avarus", "Bagger", "Pegassi Bati 801", "Pegassi Bati 801 Livery",
		"BF-400", "Nagasaki Carbon RS", "Nagasaki Chimera", "WMC Cliffhanger", "WMC Daemon",
		"WMC Daemon 2", "Shitzu Defiler", "Dinka Double-T", "Dinka Enduro", "Pegassi Esskey",
		"Principe Faggio", "Faggio Sport", "Moped Faggio", "WMC Gargoyle", "Shitzu Hakuchou",
		"Hakuchou Drag", "LCC Hexer", "LCC Innovation", "Principe Lectro", "Maibatsu Manchez",
		"Principe Nemesis", "WMC Nightblade", "Shitzu PCJ-600", "WMC Ratbike", "Pegassi Ruffian",
		"SANCHEZ", "SANCHEZ2", "SANCTUS", "SHOTARO", "SOVEREIGN",
		"Dinka Thrust", "Shitzu VADER", "Dinka Vindicator", "Pegassi Vortex", "WMC Wolfsbane",
		"WMC Zombie Chopper", "WMC Zombie Chopper2", "Oppressor 2"
	};

	inline std::vector<Hash> vehicle_motorcycle_hash
	{
		0x63abade7, 0x81e38f7f, 0x806b9cc3, 0xf9300cc5, 0xcadd5d2d,	//"AKUMA", "AVARUS", "BAGGER", "BATI", "BATI2",
		0x5283265, 0xabb0c0, 0x675ed7, 0x17420102, 0x77934cee,		//"BF400", "CARBONRS", "CHIMERA", "CLIFFHANGER", "DAEMON",
		0xac4e93c9, 0x30ff0190, 0x9c669788, 0x6882fa73, 0x794cb30c,	//"DAEMON2", "DEFILER", "DOUBLE", "ENDURO", "ESSKEY",
		0x9229e4eb, 0x350d1ab, 0xb328b188, 0x2c2c2324, 0x4b6c568a,	//"FAGGIO", "FAGGIO2", "FAGGIO3", "GARGOYLE", "HAKUCHOU",
		0xf0c2a91f, 0x11f76c14, 0xf683eaca, 0x26321e67, 0xa5325278,	//"HAKUCHOU2", "HEXER", "INNOVATION", "LECTRO", "MANCHEZ",
		0xda288376, 0xa0438767, 0xc9ceaf06, 0x6facdf31, 0xcabd11e8,	//"NEMESIS", "NIGHTBLADE", "PCJ", "RATBIKE", "RUFFIAN",
		0x2ef89e46, 0xa960b13e, 0x58e316c7, 0xe7d2a16e, 0x2c509634,	//"SANCHEZ", "SANCHEZ2", "SANCTUS", "SHOTARO", "SOVEREIGN",
		0x6d6f8f43, 0xf79a00f7, 0xaf599f01, 0xdba9dbfc, 0xdb20a373,	//"THRUST", "VADER", "VINDICATOR", "VORTEX", "WOLFSBANE",
		0xc3d7c72b, 0xde05fb87,	0x7B54A9D3							//"ZOMBIEA", "ZOMBIEB"
	};

	inline const char* vehicle_offroad[28]
	{
		"BFINJECTION", "BIFTA", "BLAZER", "BLAZER2", "BLAZER3",
		"BLAZER4", "BODHI2", "BRAWLER", "DLOADER", "DUBSTA3",
		"DUNE", "DUNE2", "INSURGENT", "INSURGENT2", "KALAHARI",
		"MARSHALL", "MESA3", "MONSTER", "RANCHERXL", "RANCHERXL2",
		"REBEL", "REBEL2", "SANDKING", "SANDKING2", "TECHNICAL",
		"TROPHYTRUCK", "TROPHYTRUCK2", "CARACARA"
	};

	inline std::vector<Hash> vehicle_offroad_hash
	{
		0x432aa566, 0xeb298297, 0x8125bcf9, 0xfd231729, 0xb44f0582,	//"BFINJECTION", "BIFTA", "BLAZER", "BLAZER2", "BLAZER3",
		0xe5ba6858, 0xaa699bb6, 0xa7ce1bc5, 0x698521e3, 0xb6410173,	//"BLAZER4", "BODHI2", "BRAWLER", "DLOADER", "DUBSTA3",
		0x9cf21e0f, 0x1fd824af, 0x9114eada, 0x7b7e56f0, 0x5852838,	//"DUNE", "DUNE2", "INSURGENT", "INSURGENT2", "KALAHARI",
		0x49863e9c, 0x84f42e51, 0xcd93a7db, 0x6210cbb0, 0x7341576b,	//"MARSHALL", "MESA3", "MONSTER", "RANCHERXL", "RANCHERXL2",
		0xb802dd46, 0x8612b64b, 0xb9210fd0, 0x3af8c345, 0x83051506,	//"REBEL", "REBEL2", "SANDKING", "SANDKING2", "TECHNICAL",
		0x612f4b6, 0xd876dbe2,	0x4ABEBF23							//"TROPHYTRUCK", "TROPHYTRUCK2"
	};

	inline const char* vehicle_coupe[14]
	{
		"Enus Cognoscenti Cabrio", "Dewbauchee Exemplar", "Ocelot F620", "Lampadati Felon", "Lampadati Felon GT",
		"Ocelot Jackal", "Ubermacht Oracle XS", "Ubermacht Oracle", "Ubermacht Sentinel XS", "Ubermacht Sentinel",
		"Enus Windsor", "Enus Windsor Drop", "Ubermacht Zion", "Ubermacht Zion Cabrio"
	};

	inline std::vector<Hash> vehicle_coupe_hash
	{
		0x13b57d8a, 0xffb15b5e, 0xdcbcbe48, 0xe8a8bda8, 0xfaad85ee,	//"COGCABRIO", "EXEMPLAR", "F620", "FELON", "FELON2",
		0xdac67112, 0x506434f6, 0xe18195b2, 0x50732c82, 0x3412ae2d,	//"JACKAL", "ORACLE", "ORACLE2", "SENTINEL", "SENTINEL2",
		0x5e4327c8, 0x8cf5cae1, 0xbd1b39c3, 0xb8e2ae18,				//"WINDSOR", "WINDSOR2", "ZION", "ZION2"
	};

	inline std::vector<Hash> vehicle_muscle_hash
	{
		0xb820ed5e, 0xd756460c, 0xc397f748, 0x14d69010, 0xaed64a63,	//"BLADE", "BUCCANEER", "BUCCANEER2", "CHINO", "CHINO2",
		0x2ec385fe, 0x4ce68ac, 0xc96b73d9, 0x2b26f456, 0xec8f7094,	//"COQUETTE3", "DOMINATOR", "DOMINATOR2", "DUKES", "DUKES2",
		0x94b395c5, 0x14d22159, 0x81a9cddf, 0x95466bdb, 0x866bce26,	//"GAUNTLET", "GAUNTLET2", "FACTION", "FACTION2", "FACTION3",
		0x239e390, 0x7b47a6a7, 0x1f52a43f, 0x710a2b9b, 0x8c2bd0dc,	//"HOTKNIFE", "LURCHER", "MOONBEAM", "MOONBEAM2", "NIGHTSHADE",
		0x831a21d5, 0x59e0fbf3, 0xd83c13ce, 0xdce1d9f7, 0xf26ceff9,	//"PHOENIX", "PICADOR", "RATLOADER", "RATLOADER2", "RUINER",
		0x9b909c94, 0xd4ea603, 0x2b7f9de3, 0x31adbbfc, 0x42bc5e19,	//"SABREGT", "SABREGT2", "SLAMVAN", "SLAMVAN2", "SLAMVAN3",
		0x72a4c31e, 0xe80f67ee, 0x39f9c898, 0xcec6b9b7, 0xe2504942,	//"STALION", "STALION2", "TAMPA", "VIGERO", "VIRGO",
		0xca62927a, 0xfdffb0, 0x779b4f2d, 0x1f3766e3, 0xC52C6B93, 0xB472D2B5 //"VIRGO2", "VIRGO3", "VOODOO", "VOODOO2"
	};

	inline const char* vehicle_muscle[41]
	{
		"BLADE", "BUCCANEER", "BUCCANEER2", "CHINO", "CHINO2",
		"COQUETTE3", "DOMINATOR", "DOMINATOR2", "DUKES", "DUKES2",
		"GAUNTLET", "GAUNTLET2", "FACTION", "FACTION2", "FACTION3",
		"HOTKNIFE", "LURCHER", "MOONBEAM", "MOONBEAM2", "NIGHTSHADE",
		"PHOENIX", "PICADOR", "RATLOADER", "RATLOADER2", "RUINER",
		"SABREGT", "SABREGT2", "SLAMVAN", "SLAMVAN2", "SLAMVAN3",
		"STALION", "STALION2", "TAMPA", "VIGERO", "VIRGO",
		"VIRGO2", "VIRGO3", "VOODOO", "VOODOO2", "DOMINATOR3", "ELLIE"
	};

	inline std::vector<Hash> vehicle_boat_hash
	{
		0x3d961290, 0x107f392c, 0x1e5e54ea, 0x33b47f96, 0x33581161,	//"DINGHY", "DINGHY2", "DINGHY3", "DINGHY4", "JETMAX",
		0xc1ce1183, 0xe2e7d4ab, 0xc2974024, 0xdb4388e4, 0xed762d49,	//"MARQUIS", "PREDATOR", "SEASHARK", "SEASHARK2", "SEASHARK3",
		0xdc60d2b, 0x1a144f2a, 0x17df5ec2, 0x2dff622f, 0xc07107ee,	//"SPEEDER", "SPEEDER2", "SQUALO", "SUBMERSIBLE", "SUBMERSIBLE2",
		0xef2295c9, 0x3fd5aa2f, 0x362cac6d, 0x1149422f, 0x56590fe9,	//"SUNTRAP", "TORO", "TORO2", "TROPIC", "TROPIC2",
		0x82cac433,													//"TUG"
	};

	inline const char* vehicle_boat[21]
	{
		"Dinghy", "Dinghy 2", "Dinghy 3", "Dinghy 4", "Jetmax",
		"Marquis", "Predator (police)", "Seashark", "Seashark 2", "Seashark 3",
		"Speeder", "Speeder 2", "Squalo", "Submarine", "Submarine 2",
		"Suntrap", "Toro", "Toro 2", "Tropic", "Tropic 2",
		"Tug"
	};

	inline std::vector<Hash> vehicle_commercial_hash
	{
		0x7a61b330, 0x32b91ae8, 0x5a82f9ae, 0x35ed670b, 0xc1632beb,	//"BENSON", "BIFF", "HAULER", "MULE", "MULE2",
		0x85a5b471, 0x21eee87d, 0x809aa4cb, 0x7de35e7d, 0x6827cf72,	//"MULE3", "PACKER", "PHANTOM", "POUNDER", "STOCKADE",
		0xf337ab36,													//"STOCKADE3"
	};

	inline const char* vehicle_commercial[11]
	{
		"Vapid Benson", "HVY Biff", "JoBuilt Hauler", "Maibatsu Mule", "Maibatsu Mule 2",
		"Maibatsu Mule 3", "MTL Packer", "JoBuilt Phantom", "MTL Pounder", "Brute Stockade",
		"Brute Stockade (Snow)"
	};

	inline std::vector<Hash> vehicle_compact_hash
	{
		0xeb70965f, 0x5c55cb39, 0xbc993509, 0x64430650, 0xb9cb3b69,	//"BLISTA", "BRIOSO", "DILETTANTE", "DILETTANTE2", "ISSI2",
		0xe644e480, 0xa988d3a2, 0x322cf98f,	0x378236E1				//"PANTO", "PRAIRIE", "RHAPSODY"
	};

	inline const char* vehicle_compact[9]
	{
		"Dinka Blista", "Grotti Brioso R/A", "Karin Dilettante", "Karin Dilettante (Patrol)", "Weeny Issi",
		"Benefactor Panto", "Bollokan Prairie", "Declasse Rhapsody", "ISSI3"
	};

	inline std::vector<Hash> vehicle_cycle_hash
	{
		0x43779c54, 0x1aba13b5, 0xce23d3bf, 0xf4e1aa15, 0x4339cd69,	//"BMX", "CRUISER", "FIXTER", "SCORCHER", "TRIBIKE",
		0xb67597ec, 0xe823fb48,										//"TRIBIKE2", "TRIBIKE3"
	};

	inline const char* vehicle_cycle[7]
	{
		"Bmx", "Cruiser", "Fixter", "Scorcher", "Whippet Race Bike",
		"Endurex Race Bike", "Tri-Cycles Race Bike"
	};

	inline std::vector<Hash> vehicle_emergency_hash
	{
		0x45d56ada, 0x432ea949, 0x9dc66994, 0x73920f8e, 0x1bf8d381,	//"AMBULANCE", "FBI", "FBI2", "FIRETRUK", "LGUARD",
		0x885f3671, 0x2c33b46e, 0x79fbb0c5, 0x9f05f101, 0x71fa16ea,	//"PBUS", "PRANGER", "POLICE", "POLICE2", "POLICE3",
		0x8a63c7b9, 0xfdefaec3, 0xa46462f7, 0x95f4c618, 0x1b38e955,	//"POLICE4", "POLICEB", "POLICEOLD1", "POLICEOLD2", "POLICET",
		0x9baa707c, 0x72935408, 0xb822a1aa,							//"SHERIFF", "SHERIFF2", "RIOT"
	};

	inline const char* vehicle_emergency[18]
	{
		"Ambulance", "FIB Buffalo", "FIB Granger", "Fire Truck", "Lifeguard Granger",
		"Prison Bus", "Park Ranger Granger", "Police Cruiser", "Police Cruiser 2", "Police Cruiser 3",
		"Unmarked Cruiser", "Police Bike", "Police Cruiser Snow", "Police Cruiser Snow 2", "Police Transporter",
		"Sheriff Cruiser", "Sheriff SUV", "Police Riot"
	};

	inline std::vector<Hash> vehicle_helicopter_hash
	{
		0x31f0b376, 0xf7004c86, 0xdb6b4924, 0x2f03547b, 0x2c75f0dd,	//"ANNIHILATOR", "BLIMP", "BLIMP2", "BUZZARD", "BUZZARD2",
		0xfcfcb68b, 0x60a7ea10, 0x53174eef, 0x78bc1a3c, 0x2c634fbd,	//"CARGOBOB", "CARGOBOB2", "CARGOBOB3", "CARGOBOB4", "FROGGER",
		0x742e9ac0, 0x9d0450ca, 0x1517d4d9, 0xfb133a17, 0x3e48bf23,	//"FROGGER2", "MAVERICK", "POLMAV", "SAVAGE", "SKYLIFT",
		0x2a54c47d, 0x9c5e5644, 0xebc24df2, 0x4019cb4c, 0xa09e15fd,	//"SUPERVOLITO", "SUPERVOLITO2", "SWIFT", "SWIFT2", "VALKYRIE",
		0x5bfa5c4b, 0x920016f1,	0xD4AE63D9, 0xEDA4ED97				//"VALKYRIE2", "VOLATUS"
	};

	inline const char* vehicle_helicopter[24]
	{
		"ANNIHILATOR", "Blimp", "Blimp 2", "BUZZARD", "BUZZARD2",
		"CARGOBOB", "CARGOBOB2", "CARGOBOB3", "CARGOBOB4", "FROGGER",
		"FROGGER2", "MAVERICK", "POLMAV", "SAVAGE", "SKYLIFT",
		"SUPERVOLITO", "SUPERVOLITO2", "SWIFT", "SWIFT2", "VALKYRIE",
		"VALKYRIE2", "VOLATUS", "SEASPARROW", "Blimp 3"
	};

	inline std::vector<Hash> vehicle_industrial_hash
	{
		0x7074f39d, 0xc3fba120, 0x810369e2, 0x50b0215a, 0x825a9f4c,	//"BULLDOZER", "CUTTER", "DUMP", "FLATBED", "GUARDIAN",
		0x1a7fcefa, 0xd138a6bb, 0x1c534995, 0x9a5b1dcc, 0x2e19879,	//"HANDLER", "MIXER", "MIXER2", "RUBBLE", "TIPTRUCK",
		0xc7824e5e,													//"TIPTRUCK2"
	};

	inline const char* vehicle_industrial[11]
	{
		"HVY Dozer", "HVY Cutter", "HVY Dump", "MTL Flatbed", "Vapid Guardian",
		"Dock Handler", "HVY Mixer", "HVY Mixer 2", "JoBuilt Rubble", "Brute Tipper",
		"Tipper"
	};

	inline std::vector<Hash> vehicle_military_hash
	{
		0xceea3f4b, 0x4008eabb, 0x2592b5cf, 0x132d5a1a, 0x2ea68690,	//"BARRACKS", "BARRACKS2", "BARRACKS3", "CRUSADER", "RHINO"
	};

	inline const char* vehicle_military[5] =
	{
		"Barracks", "HVY Barracks Semi", "Barracks 3", "Canis Crusader", "Rhino Tank"
	};

	inline std::vector<Hash> vehicle_plane_hash
	{
		0x6cbd1d6d, 0x15f27762, 0xd9927fe3, 0xca495705, 0x39d6779e,	//"BESRA", "CARGOPLANE", "CUBAN800", "DODO", "DUSTER",
		0x39d6e83f, 0x3f119114, 0xb39b0ae6, 0x250b0c5e, 0xb79f589e,	//"HYDRA", "JET", "LAZER", "LUXOR", "LUXOR2",
		0x97e55d11, 0x9d80f93, 0xb2cf7250, 0xb79c1bf5, 0x81794c70,	//"MAMMATUS", "MILJET", "NIMBUS", "SHAMAL", "STUNT",
		0x761e2ad3, 0x9c429b6a, 0x403820e8, 0x4ff77e37,				//"TITAN", "VELUM", "VELUM2", "VESTRA"
	};

	inline const char* vehicle_plane[19]
	{
		"BESRA", "CARGOPLANE", "CUBAN800", "DODO", "DUSTER",
		"HYDRA", "JET", "LAZER", "LUXOR", "LUXOR2",
		"MAMMATUS", "MILJET", "NIMBUS", "SHAMAL", "STUNT",
		"TITAN", "VELUM", "VELUM2", "VESTRA"
	};

	inline std::vector<Hash> vehicle_service_hash
	{
		0x4c80eb0e, 0xedc6f847, 0xd577c962, 0x84718d34, 0x829a3c44,	//"AIRBUS", "BRICKADE", "BUS", "COACH", "RALLYTRUCK",
		0xbe819c63, 0xc703db5f, 0x73b1c3cb, 0x72435a19, 0xb527915c,	//"RENTALBUS", "TAXI", "TOURBUS", "TRASH", "TRASH2"
	};

	inline const char* vehicle_service[10]
	{
		"Airport Bus", "MTL Brickade", "Bus", "Dashound", "MTL Dune",
		"Rental Shuttle Bus", "Taxi", "Tourbus", "Trashmaster", "Trashmaster 2"
	};

	inline std::vector<Hash> vehicle_suv_hash
	{
		0xcfca3668, 0x8852855, 0x6ff0f727, 0x25cbe2e2, 0x1c09cf5e,	//"BALLER", "BALLER2", "BALLER3", "BALLER4", "BALLER5",
		0x27b4e6b0, 0x32b29a4b, 0x779f23aa, 0xd0eb2be5, 0x28b67aca,	//"BALLER6", "BJXL", "CAVALCADE", "CAVALCADE2", "CONTENDER",
		0x462fe277, 0xe882e5f6, 0xbc32a33b, 0x9628879c, 0xa3fc0f4d,	//"DUBSTA", "DUBSTA2", "FQ2", "GRANGER", "GRESLEY",
		0x34b7390f, 0x1d06d681, 0x4ba4e8dc, 0x36848602, 0xd36a4b44,	//"HABANERO", "HUNTLEY", "LANDSTALKER", "MESA", "MESA2",
		0xcfcfeb3b, 0x9d96b45b, 0x7f5c91f1, 0x48ceced3, 0x4fb1a214,	//"PATRIOT", "RADI", "ROCOTO", "SEMINOLE", "SERRANO",
		0x47bbcf2e, 0xe6401328,	0x79DD18AE, 0xE6E967F8				//"XLS", "XLS2"
	};

	inline const char* vehicle_suv[29] =
	{
		"Gallivanter Baller", "Gallivanter Baller 2nd Gen", "Gallivanter Baller 3", "Gallivanter Baller 4", "Gallivanter Baller 4",
		"Gallivanter Baller 6", "Karin BeeJay XL", "Albandy Cavalcade", "Albandy Cavalcade 2nd Generation", "Vapid Contender",
		"Benefactor Dubsta", "Benefactor Dubsta Armored", "Fathom FQ 2", "Declasse Granger", "Bravado Gresley",
		"Emperor Habanero", "Huntley", "Landstalker", "Mesa Grande", "Merryweather Mesa",
		"Patriot", "Radi", "Rocoto", "Canis Seminole", "Benefactor Serrano",
		"Benefactor XLS", "Benefactor XLS Armored", "Menacer", "Limo Patriot"
	};

	inline std::vector<Hash> vehicle_trailer_hash
	{
		0xb8081009, 0xa7ff33f5, 0x9e6b14d6, 0xe82ae656, 0x1f3d44b5,	//"ARMYTANKER", "ARMYTRAILER", "ARMYTRAILER2", "BALETRAILER", "BOATTRAILER",
		0x806efbee, 0xd1abb666, 0x3cc7f596, 0x153e1b0a, 0x174cb172,	//"DOCKTRAILER", "FREIGHTTRAILER", "GRAINTRAILER", "PROPTRAILER", "RAKETRAILER",
		0xd46f4737, 0x74998082, 0x7be032c6, 0x6a59902d, 0x7cab34d0,	//"TANKER", "TANKER2", "TR2", "TR3", "TR4",
		0x782a236d, 0xcbb2be0e, 0xa1da3c91, 0x8548036d, 0x2a72beab,	//"TRAILERLOGS", "TRAILERS", "TRAILERS2", "TRAILERS3", "TRAILERSMALL",
		0xaf62f6b2, 0x967620be,										//"TRFLAT", "TVTRAILER"
	};

	inline const char* vehicle_trailer[22]
	{
		"ARMYTANKER", "ARMYTRAILER", "ARMYTRAILER2", "BALETRAILER", "BOATTRAILER",
		"DOCKTRAILER", "FREIGHTTRAILER", "GRAINTRAILER", "PROPTRAILER", "RAKETRAILER",
		"TANKER", "TANKER2", "TR2", "TR3", "TR4",
		"TRAILERLOGS", "TRAILERS", "TRAILERS2", "TRAILERS3", "TRAILERSMALL",
		"TRFLAT", "TVTRAILER"
	};

	inline std::vector<Hash> vehicle_train_hash
	{
		0xc6c3242d, 0x3d6aaa9b, 0xafd22a6, 0x36dcff98, 0xe512e79,	//"CABLECAR", "FREIGHT", "FREIGHTCAR", "FREIGHTCONT1", "FREIGHTCONT2",
		0x264d9262, 0x33c9e158, 0x22eddc30,							//"FREIGHTGRAIN", "METROTRAIN", "TANKERCAR"
	};

	inline const char* vehicle_train[8]
	{
		"CABLECAR", "FREIGHT", "FREIGHTCAR", "FREIGHTCONT1", "FREIGHTCONT2",
		"FREIGHTGRAIN", "METROTRAIN", "TANKERCAR"
	};

	inline std::vector<Hash> vehicle_utility_hash
	{
		0x5d0aac8f, 0x44623884, 0xdff0594c, 0xcb44b1ca, 0x58e49664,	//"AIRTUG", "CADDY", "CADDY2", "DOCKTUG", "FORKLIFT",
		0x6a4bd8f6, 0xcd935ef9, 0xdc434e51, 0x2bc345d1, 0x9a9fd3df,	//"MOWER", "RIPLEY", "SADLER", "SADLER2", "SCRAP",
		0xb12314e0, 0xe5a2d6c6, 0x61d6ba8c, 0x843b73de, 0x562a97bd,	//"TOWTRUCK", "TOWTRUCK2", "TRACTOR", "TRACTOR2", "TRACTOR3",
		0x1ed0a534, 0x34e6bf6b, 0x7f2153df,							//"UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3"
	};

	inline const char* vehicle_utility[18]
	{
		"AIRTUG", "CADDY", "CADDY2", "DOCKTUG", "FORKLIFT",
		"MOWER", "RIPLEY", "SADLER", "SADLER2", "SCRAP",
		"TOWTRUCK", "TOWTRUCK2", "TRACTOR", "TRACTOR2", "TRACTOR3",
		"UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3"
	};

	inline std::vector<Hash> vehicle_van_hash
	{
		0xfefd644f, 0x7b8297c5, 0x67b3f020, 0x3fc5d440, 0x898eccea,	//"BISON", "BISON2", "BISON3", "BOBCATXL", "BOXVILLE",
		0xf21b33be, 0x7405e08, 0x1a79847a, 0xafbb2ca4, 0xc9e8ff76,	//"BOXVILLE2", "BOXVILLE3", "BOXVILLE4", "BURRITO", "BURRITO2",
		0x98171bd3, 0x353b561d, 0x437cf2a0, 0x6fd95f68, 0x97fa4f36,	//"BURRITO3", "BURRITO4", "BURRITO5", "CAMPER", "GBURRITO",
		0x11aa0e14, 0xf8d48e7a, 0xed7eada4, 0xbcde91f0, 0x58b3979c,	//"GBURRITO2", "JOURNEY", "MINIVAN", "MINIVAN2", "PARADISE",
		0xf8de29a8, 0x38408341, 0x4543b74d, 0x961afef7, 0x57f682af,	//"PONY", "PONY2", "RUMPO", "RUMPO2", "RUMPO3",
		0xcfb3870c, 0x2b6dc64a, 0x29b0da97, 0xb1d80e06, 0x744ca80d,	//"SPEEDO", "SPEEDO2", "SURFER", "SURFER2", "TACO",
		0x3e5f6b8, 0x3d29cd2b,										//"YOUGA", "YOUGA2"
	};

	inline const char* vehicle_van[32]
	{
		"BISON", "BISON2", "BISON3", "BOBCATXL", "BOXVILLE",
		"BOXVILLE2", "BOXVILLE3", "BOXVILLE4", "BURRITO", "BURRITO2",
		"BURRITO3", "BURRITO4", "BURRITO5", "CAMPER", "GBURRITO",
		"GBURRITO2", "JOURNEY", "MINIVAN", "MINIVAN2", "PARADISE",
		"PONY", "PONY2", "RUMPO", "RUMPO2", "RUMPO3",
		"SPEEDO", "SPEEDO2", "SURFER", "SURFER2", "TACO",
		"YOUGA", "YOUGA2"
	};

	inline const char* vehicle_casinoheist[20] = {
		"Abso", "Everon", "Formula", "Formula2", "Furia",
		"Imorgon", "JB7002", "Kanjo", "Komoda", "Minitank",
		"Outlaw", "Rebla", "Retinue 2", "Stryder", "Sugoi",
		"Sultan 2", "Vagrant", "VSTR", "Yosemite 2", "Zhaba"
	};

	inline std::vector<Hash> vehicle_casinoheist_hash
	{
		RAGE_JOAAT("ABSO"), RAGE_JOAAT("EVERON"), RAGE_JOAAT("FORMULA"), RAGE_JOAAT("FORMULA2"), RAGE_JOAAT("FURIA"),
		RAGE_JOAAT("IMORGON"), RAGE_JOAAT("JB7002"), RAGE_JOAAT("KANJO"), RAGE_JOAAT("KOMODA"), RAGE_JOAAT("MINITANK"),
		RAGE_JOAAT("OUTLAW"), RAGE_JOAAT("REBLA"), RAGE_JOAAT("RETINUE2"), RAGE_JOAAT("STRYDER"), RAGE_JOAAT("SUGOI"),
		RAGE_JOAAT("SULTAN2"), RAGE_JOAAT("VAGRANT"), RAGE_JOAAT("VSTR"), RAGE_JOAAT("YOSEMITE2"), RAGE_JOAAT("ZHABA"),
	};

	inline const char* vehicle_casino[22] = {
		"Vapid Caracara 4x4", "Obey 8F Drafter", "Weeny Dynasty", "Progen Emerus", "Bravado Gauntlet Classic", "Bravado Gauntlet Hellfire", "Annis Hellion", "Weeny Issi Sport", "Ocelot Jugular",
		"Benefactor Krieger", "Ocelot Locust", "Vulcar Nebula Turbo", "Vysser Neo", "Lampadati Novak", "Enus Paragon R", "Enus Paragon R (Armored)", "Vapid Peyote Gasser", "Western Rampant Rocket",
		"Annis S80 RR", "Truffade Thrax", "Ubermacht Zion Classic", "Pegassi Zorrusso"
	};

	inline std::vector<Hash> vehicle_casino_hash
	{
		0xAF966F3C, 0x28EAB80F, 0x127E90D5, 0x4EE74355, 0x2B0C4DCD,
		0x734C5E50, 0xEA6A047F, 0x6E8DA4F7, 0xF38C4245, 0xD86A0247,
		0xC7E55211, 0xCB642637, 0x9F6ED5A2, 0x92F5024E, 0xE550775B,
		0x546D8EEE, 0x9472CD24, 0x36A167E0, 0xECA6B6A3, 0x3E3D1F59,
		0x6F039A67, 0xD757D97D
	};

	inline const char* vehicle_arenawars[45] = {
		"Apocalypse Bruiser", "Future Shock Bruiser", "Nightmare Bruiser", "Apocalypse Brutus", "Future Shock Brutus", "Nightmare Brutus", "Apocalypse Cerberus", "Future Shock Cerberus", "Nightmare Cerberus",
		"Clique", "Apocalypse Deathbike", "Future Shock Deathbike", "Nightmare Deathbike", "Deveste", "Deviant GT", "Apocalypse Dominator", "Future Shock  Dominator", "Nightmare Dominator",
		"Impaler", "Apocalypse Impaler", "Future Shock Impaler", "Nightmare Impaler", "Apocalypse Imperator", "Future Shock Imperator", "Nightmare Imperator", "Apocalypse Issi", "Future Shock Issi", "Nightmare Issi",
		"Itali GTO", "Apocalypse Sasquatch", "Future Shock Sasquatch", "Nightmare Sasquatch", "RC Bandito", "Future Shock Scarab", "Nightmare Scarab", "Schlagen", "Apocalypse Slamvan", "Future Shock Slamvan",
		"Nightmare Slamvan", "Toros", "Tulip", "Vamos", "Apocalypse ZR 380", "Future Shock ZR 380", "Nightmare ZR 380"
	};

	inline std::vector<Hash> vehicle_arenawars_hash
	{
		0x27D79225, 0x9B065C9E, 0x8644331A, 0x7F81A829, 0x8F49AE28,
		0x798682A2, 0xD039510B, 0x287FA449, 0x71D3B6F0, 0xA29F78B0,
		0xFE5F0722, 0x93F09558, 0xAE12C99C, 0x5EE005DA, 0x4C3FFF49,
		0xD6FB0F30, 0xAE0A3D4F, 0xB2E046FB, 0x83070B62, 0x3C26BD0C,
		0x8D45DF49, 0x9804F4C7, 0x1A861243, 0x619C1B82, 0xD2F77E37,
		0x256E92BA, 0x5BA0FF1E, 0x49E25BA1, 0xEC3E3404, 0x669EB40A,
		0x32174AFC, 0xD556917C, 0xEEF345EC, 0x5BEB3CE0, 0xDD71BFEB,
		0xE1C03AB0, 0x8526E2F5, 0x163F8520, 0x67D52852, 0xBA5334AC,
		0x56D42971, 0xFD128DFD, 0x20314B42, 0xBE11EFC6, 0xA7DCC35C
	};

	inline const char* vehicle_afterhours[14] = {
		"Blimp",
		"Freecrawler",
		"Menacer",
		"Mule Custom",
		"Oppressor MK II",
		"Patriot Stretch",
		"Festival Bus",
		"Pounder Custom",
		"Scramjet",
		"Speedo Custom",
		"Stafford",
		"B-11 Strikeforce",
		"Swinger",
		"Terrorbyte",
	};

	inline std::vector<Hash> vehicle_afterhours_hash
	{
		0xEDA4ED97, 0xFCC2F483, 0x79DD18AE, 0x73F4110E, 0x7B54A9D3,
		0xE6E967F8, 0x149BD32A, 0x6290F15B, 0xD9F0503D, 0x0D17099D,
		0x1324E960, 0x64DE07A1, 0x1DD4C0FF, 0x897AFC65
	};

	inline const char* vehicle_ssas[16] = {
		"Caracara",
		"Cheburek",
		"Dominator GTX",
		"Ellie",
		"Entity XXR",
		"Fagaloa",
		"Flash GT",
		"GB200",
		"Hotring Sabre",
		"Issi Classic",
		"Jester Classic",
		"Michelli GT",
		"Sea Sparrow",
		"Taipan",
		"Tezeract",
		"Tyrant",
	};

	inline std::vector<Hash> vehicle_ssas_hash
	{
		0x4ABEBF23,
		0xC514AAE0,
		0xC52C6B93,
		0xB472D2B5,
		0x8198AEDC,
		0x6068AD86,
		0xB4F32118,
		0x71CBEA98,
		0x42836BE5,
		0x378236E1,
		0xF330CB6A,
		0x3E5BD8D9,
		0xD4AE63D9,
		0xBC5DC07E,
		0x3D7C6410,
		0xE99011C2,
	};

	inline const char* vehicle_doomsday[28] = {
		"Autarch", "Avenger", "Barrage", "Chernobog", "Comet Safari", "Comet SR",
		"Deluxo", "GT500", "Hermes", "Hustler", "Kamacho","TM-02 Khanjali",
		"Neon", "Pariah", "Raiden", "Revolter", "Riata", "RCV", "Savestra",
		"SC1", "Sentinel Classic", "Streiter", "Stromberg", "Thruster (Jetpack)", "Viseris",
		"Volatol", "Yosemite", "190z"
	};

	inline const char* vehicle_doomsday_hash[28] = {
		"AUTARCH", "AVENGER", "BARRAGE", "CHERNOBOG", "COMET4", "COMET5",
		"DELUXO", "GT500", "HERMES", "HUSTLER", "KAMACHO","KHANJALI",
		"NEON", "PARIAH", "RAIDEN", "REVOLTER", "RIATA", "RIOT2", "SAVESTRA",
		"SC1", "SENTINEL3", "STREITER", "STROMBERG", "THRUSTER", "VISERIS",
		"VOLATOL", "YOSEMITE", "Z190"
	};

	inline const char* vehicle_smugglers[19] = {
		"Havok", "Rapid GT Classic", "Retinue", "Vigilante", "Visione", "Cyclone", "Alpha-Z1",
		"RM-10 Bombushka", "Howard NX-25", "FH-1 Hunter", "Ultralight", "Mogul", "V-65 Molotok", "P-45 Nokota", "Pyro",
		"Rogue", "Seabreeze", "LF-22 Starling", "Tula"
	};

	inline const char* vehicle_smugglers_hash[19] = {
		"HAVOK", "RAPIDGT3", "RETINUE", "VIGILANTE", "VISIONE", "CYCLONE", "ALPHAZ1",
		"BOMBUSHKA", "HOWARD", "HUNTER", "MICROLIGHT", "MOGUL", "MOLOTOK", "NOKOTA", "PYRO",
		"ROGUE", "SEABREEZE", "STARLING", "TULA"
	};

	inline const char* vehicle_gunrunning[19] = {
		"APC", "Ardent", "Caddy", "Cheetah Classic", "Dune FAV", "Half-Track", "Hauler Custom",
		"Insurgent Pick-Up Custom", "Nightshark", "Oppressor MK I", "Phantom Custom", "Weaponized Tampa", "Technical Custom",
		"Torero", "MOC Trailer", "Trailer Classic", "Anti-Aircraft Trailer", "Vagner", "XA-21"
	};

	inline const char* vehicle_gunrunning_hash[19] = {
		"APC", "ARDENT", "CADDY3", "CHEETAH2", "DUNE3", "HALFTRACK", "HAULER2",
		"INSURGENT3", "NIGHTSHARK", "OPPRESSOR", "PHANTOM3", "TAMPA3", "TECHNICAL3",
		"TORENO", "TRAILERLARGE", "TRAILERS4", "TRAILERSMALL2", "VAGNER", "XA21"
	};

	inline const char* vehicle_cunningstunts[15] = {
		"BF400", "BRIOSO", "CLIFFHANGER", "CONTENDER", "GARGOYLE", "LE7B", "LYNX",
		"OMNIS", "RALLYTRUCK", "SHEAVA", "TAMPA2", "TROPHYTRUCK", "TROPHYTRUCK2",
		"TROPOS", "TYRUS"
	};

	inline const char* vehicle_cunningstunts_hash[15] = {
		"BF-400", "Brioso", "Cliff Hanger", "Contender", "Gargoyle", "LE-7B", "Lynx",
		"Omnis", "Rally Truck", "Sheava", "TAMPA2", "TROPHYTRUCK", "TROPHYTRUCK2",
		"Tropos", "Tyrus"
	};

	inline const char* vehicle_importexport[23] = {
		"BLAZER5", "BOXVILLE5", "COMET3", "DIABLOUS", "DIABLOUS2", "DUNE4", "DUNE5",
		"ELEGY", "FCR", "FCR2", "ITALIGTB", "ITALIGTB2", "NERO", "NERO2", "PENETRATOR",
		"PHANTOM2", "RUINER2", "SPECTER", "SPECTER2", "TECHNICAL2", "TEMPESTA", "VOLTIC2",
		"WASTELANDER"
	};

	inline const char* vehicle_importexport_hash[23] = {
		"BLAZER5", "BOXVILLE5", "COMET3", "DIABLOUS", "DIABLOUS2", "DUNE4", "DUNE5",
		"ELEGY", "FCR", "FCR2", "ITALIGTB", "ITALIGTB2", "NERO", "NERO2", "PENETRATOR",
		"PHANTOM2", "RUINER2", "SPECTER", "SPECTER2", "TECHNICAL2", "TEMPESTA", "VOLTIC2",
		"WASTELANDER"
	};



	inline std::map<signed int, std::string> Weapons{
		{ 453432689, "No Weapon" },
		{ 1198256469, "Unholy Hellbringer"},
		{ -1716189206, "Knife" },
		{ 1737195953, "Nightstick" },
		{ 1317494643, "Hammer" },
		{ -1786099057, "Bat" },
		{ -2067956739, "Crowbar" },
		{ 1141786504, "Golfclub" },
		{ -102323637, "Bottle" },
		{ -1834847097, "Dagger" },
		{ -102973651, "Hatchet" },
		{ -656458692, "Knuckle Duster" },
		{ -581044007, "Machete" },
		{ -1951375401, "Flashlight"},
		{ -538741184, "Switch Blade"},
		{ -1810795771, "Poolcue"},
		{ 419712736, "Wrench"},
		{ -853065399, "Battleaxe"},
		{ 453432689, "Pistol"},
		{ 3219281620, "Pistol Mk2"},
		{ 1593441988, "Combat Pistol"},
		{ -1716589765, "Pistol 50"},
		{ -1076751822, "SNS Pistol"},
		{ -771403250, "Heavy Pistol"},
		{ 137902532, "Vintage Pistol"},
		{ -598887786, "Marksman Pistol"},
		{ -1045183535, "Revolver"},
		{ 584646201, "AP Pistol"},
		{ 911657153, "Stun Gun"},
		{ 1198879012, "Flare Gun"},
		{ 324215364, "Micro SMG"},
		{ -619010992, "Machine Pistol"},
		{ 736523883, "SMG"},
		{ 2024373456, "SMG Mk2"},
		{ -270015777, "Assault SMG"},
		{ 171789620, "Combat PDW"},
		{ -1660422300, "MG"},
		{ 2144741730, "Combat MG"},
		{ 3686625920, "Combat MGMk2"},
		{ 1627465347, "Gusenberg"},
		{ -1121678507, "Mini SMG"},
		{ -1074790547, "Assault Rifle"},
		{ 961495388, "Assault Rifle Mk2"},
		{ -2084633992, "Carbine Rifle"},
		{ 4208062921, "Carbine Rifle Mk2"},
		{ -1357824103, "Advanced Rifle"},
		{ -1063057011, "Special Carbine"},
		{ 2132975508, "Bullpup Rifle"},
		{ 1649403952, "Compact Rifle"},
		{ 100416529, "Sniper Rifle"},
		{ 205991906, "Heavy Sniper"},
		{ 177293209, "Heavy Sniper Mk2"},
		{ -952879014, "Marksman Rifle"},
		{ 487013001, "Pump Shotgun"},
		{ 2017895192, "Sawnoff Shotgun"},
		{ -1654528753, "Bullpup Shotgun"},
		{ -494615257, "Assault Shotgun"},
		{ -1466123874, "Musket"},
		{ 984333226, "Heavy Shotgun"},
		{ -275439685, "Double Barrel Shotgun"},
		{ 317205821, "Auto Shotgun"},
		{ -1568386805, "Grenade Launcher"},
		{ -1312131151, "RPG"},
		{ 1119849093, "Minigun"},
		{ 2138347493, "Firework"},
		{ 1834241177, "Railgun"},
		{ 1672152130, "Homing Launcher"},
		{ 1305664598, "Grenade Launcher Smoke"},
		{ 125959754, "Compact Launcher"},
		{ -1813897027, "Grenade"},
		{ 741814745, "Sticky Bomb"},
		{ -1420407917, "Proximity Mine"},
		{ -1600701090, "BZ Gas"},
		{ 615608432, "Molotov"},
		{ 101631238, "Fire Extinguisher"},
		{ 883325847, "Petrol Can"},
		{ 1233104067, "Flare"},
		{ 600439132, "Ball"},
		{ 126349499, "Snowball"},
		{ -37975472, "Smoke Grenade"},
		{ -1169823560, "Pipebomb"},
		{ -72657034, "Parachute"}
	};
}
	