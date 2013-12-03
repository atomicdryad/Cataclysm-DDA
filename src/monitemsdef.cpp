#include "game.h"
#include "setvector.h"
#include "json.h"

void game::load_monitem(JsonObject &jo) {
    std::vector<std::string> tmp_keys;
    std::vector<items_location_and_chance> tmp_items;
    std::string mkey="";

    if ( jo.has_string("id") ) {
        tmp_keys.push_back( jo.get_string("id") );
    } else if ( jo.has_array("id") ) {
        jo.read("id", tmp_keys);
    } else {
        jo.throw_error("monitems: requires \"id\": \"monster_id\" or \"id\": [ \"multiple\", \"monster_ids\" ]");
    }

    if ( ! jo.has_array("item_groups") ) {
        jo.throw_error("monitems: requires \"item_groups\": [ [ \"group_one\", (chance) ], ... ]");
    }

    JsonArray ja = jo.get_array("item_groups");
    JsonArray ga;
    while ( ja.has_more() ) {
        ga = ja.next_array();
        if ( ! ga.has_string(0) || ! ga.has_number(1) ) {
             jo.throw_error("monitems: item_groups must contain arrays of [ \"string\", number ]");
        }
        tmp_items.push_back( items_location_and_chance( ga.get_string(0), ga.get_int(1) ) );
    }

    for( int i = 0; i < tmp_keys.size(); i++ ) {
        std::map<std::string, std::vector <items_location_and_chance> >::iterator it = monitems.find( tmp_keys[i] );
        if ( it == monitems.end() ) {
            monitems[ tmp_keys[i] ] = tmp_items;
        } else {
            it->second.insert( it->second.end(), tmp_items.begin(), tmp_items.end() );
        }
    }
}

/* tmp
=======

void game_init_monitems() {

    // This seems like a good candidate for json.

    // for zombies only: clothing is generated separately upon mondeath
    // monitems should therefore not include main clothing items but extra items
    // that zombies might be carrying

    setvector(&monitems["mon_zombie"],
              "livingroom", 5, "kitchen",  4, "bedroom", 16,
              "softdrugs",  5, "harddrugs", 1, "tools", 20, "trash", 7,
              "ammo", 1, "pistols", 1, "shotguns", 1, "smg", 1,
              NULL);

    setvector(&monitems["mon_zombie_child"], "child_items", 65, NULL);

    monitems["mon_zombie_shrieker"]   = monitems["mon_zombie"];
    monitems["mon_zombie_spitter"]    = monitems["mon_zombie"];
    monitems["mon_zombie_electric"]   = monitems["mon_zombie"];
    monitems["mon_zombie_brute"]      = monitems["mon_zombie"];
    monitems["mon_zombie_hulk"]       = monitems["mon_zombie"];
    monitems["mon_zombie_fungus"]     = monitems["mon_zombie"];
    monitems["mon_boomer"]            = monitems["mon_zombie"];
    monitems["mon_boomer_fungus"]     = monitems["mon_zombie"];
    monitems["mon_zombie_necro"]      = monitems["mon_zombie"];
    monitems["mon_zombie_grabber"]    = monitems["mon_zombie"];
    monitems["mon_zombie_master"]     = monitems["mon_zombie"];
    monitems["mon_zombie_hunter"]     = monitems["mon_zombie"];
	monitems["mon_zombie_tough"]      = monitems["mon_zombie"];
	monitems["mon_zombie_rot"]        = monitems["mon_zombie"];
	monitems["mon_zombie_crawler"]    = monitems["mon_zombie"];

	setvector(&monitems["mon_zombie_fat"],
              "livingroom", 5, "fast_food",  25, "bedroom", 10,
              "softdrugs",  5, "harddrugs", 1, "tools", 4, "trash", 7,
              "ammo", 1, "pistols", 1, "shotguns", 1, "smg", 1,
              NULL);
	
	setvector(&monitems["mon_zombie_hazmat"], "rad_gear",  4, NULL);
	
    setvector(&monitems["mon_beekeeper"], "hive", 80, NULL);
    setvector(&monitems["mon_zombie_cop"], "cop_weapons", 20, NULL);

    setvector(&monitems["mon_zombie_scientist"],
              "harddrugs", 6, "chem_lab", 10,
              "teleport", 6, "goo", 8, "cloning_vat", 1,
              "dissection", 10, "electronics", 9, "bionics", 1,
              "radio", 2, "textbooks", 3, NULL);

    setvector(&monitems["mon_zombie_soldier"],
              "ammo", 10, "pistols", 5,
              "shotguns", 2, "smg", 5, "bots", 1,
              "launchers", 2, "mil_rifles", 10, "grenades", 5,
              "mil_accessories", 10, "mil_food", 5, "bionics_mil", 1,
              NULL);			  
			  
    setvector(&monitems["mon_biollante"], "biollante", 1, NULL);

    setvector(&monitems["mon_chud"],
              "subway", 40,"sewer", 20,"trash",  5,"bedroom",  1,
              "dresser",  5,"ammo", 18, NULL);
    monitems["mon_one_eye"]  = monitems["mon_chud"];

    setvector(&monitems["mon_bee"], "bees", 1, NULL);
    setvector(&monitems["mon_wasp"], "wasps", 1, NULL);
    setvector(&monitems["mon_eyebot"], "robots", 4, "eyebot", 1, NULL);
    setvector(&monitems["mon_manhack"], "robots", 4, "manhack", 1, NULL);
    setvector(&monitems["mon_skitterbot"], "robots", 4, "skitterbot", 1, NULL);
    setvector(&monitems["mon_secubot"], "robots", 4, "secubot", 1, NULL);
    setvector(&monitems["mon_copbot"], "robots", 4, "copbot", 1, NULL);
    setvector(&monitems["mon_molebot"], "robots", 4, "molebot", 1, NULL);
    setvector(&monitems["mon_tripod"], "robots", 4, "tripod", 1, NULL);
    setvector(&monitems["mon_chickenbot"], "robots", 4, "chickenbot", 1, NULL);
    setvector(&monitems["mon_tankbot"], "robots", 4, "tankbot", 1, NULL);
    setvector(&monitems["mon_turret"], "robots", 10, "turret", 1, NULL);
    setvector(&monitems["mon_laserturret"], "robots", 10, "laserturret", 1, NULL);
    setvector(&monitems["mon_fungal_fighter"], "fungal_sting", 1, NULL);
    setvector(&monitems["mon_shia"], "shia_stuff", 1, NULL);
	setvector(&monitems["mon_dog_zombie_cop"], "dog_cop", 1, NULL);
}
*/
