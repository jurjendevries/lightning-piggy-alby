#include "Constants.h"

String foundWalletID = "";

String lnurlPayments[MAX_PAYMENTS];
int nroflnurlPayments = 0;

int getWalletBalance() {
  Serial.println("Getting wallet details...");
  const String url = "/api/v1/wallet";

  int balanceBiasInt = getBalanceBiasAsInt();

  #ifdef DEBUG
  return 12345678 + balanceBiasInt;
  #endif

  const String line = getEndpointData(lnbitsHost, url, true);
  Serial.println("Got wallet balance line: " + line);
  DynamicJsonDocument doc(4096); // 4096 bytes is plenty for just the wallet details (id, name and balance info)

  DeserializationError error = deserializeJson(doc, line);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return NOT_SPECIFIED;
  }

  String walletName = doc["name"];

  if (walletName == "null") {
    Serial.println("ERROR: could not find wallet details on lnbits host " + String(lnbitsHost) + " with invoice/read key " + String(invoiceKey) + " so something's wrong! Did you make a typo?");
    return NOT_SPECIFIED;
  } else {
    Serial.print("Wallet name: " + walletName);
  }

  int walletBalance = doc["balance"];
  walletBalance = walletBalance / 1000;

  Serial.println(" contains " + String(walletBalance) + " sats");
  return walletBalance+balanceBiasInt;
}

void fetchLNURLPayments(int limit) {
  const String url = "/api/v1/payments?limit=" + String(limit);
  Serial.println("Getting payments from " + String(url));

  #ifdef DEBUG
  // To escape quotes and newlines, use:
  // sed 's/"/\\"/g' | sed "s/\\\n/\\\\\\\n/g"
  // regular testcase with long comment and special characters:
  //const String line = "[{\"checking_id\":\"eae170927e1e30811cb242a47436ec46aff9fbc73409079ce37f3e888bd45f7b\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"piggytest\",\"time\":1690630482,\"bolt11\":\"lnbc10n1pjvf72jsp5w99r5sg4kqnhjl2syltkwxf2gm86p0p3mh2rm5fwahxmwr047l3spp5atshpyn7rccgz89jg2j8gdhvg6hln778xsys088r0ulg3z75taashp5gxwncgtpe3jmwprje9eyysh7ap0xe2ez8uy59s436xftc9vd0cdqxqzjccqpjrzjqdjs3alg9wmchtfs0nav7nkls58u8usv5pc742q8fkw4gf9fpykqkzahvuqq2sgqqyqqqqqqqqqqqeqqjq9qxpqysgqttwctdvcq64s5tv0qemcykhw4atv7l3nta0029z75ve35xxk03mp6q2cs5yznnwp0euchrq7tw8heg98p7xylq9cl5jmd45r55zttfcp83pzun\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"eae170927e1e30811cb242a47436ec46aff9fbc73409079ce37f3e888bd45f7b\",\"expiry\":1690631082.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"Dein Körper € für Üeb Wärme großes, so let's see how it shows up. Woohoow! Looking great!\"],\"wh_status\":404,\"wh_success\":false,\"wh_message\":\"Not Found\",\"wh_response\":\"response\"},\"wallet_id\":\"12345678901234567890123456789012\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"d5c3a87176a0a89ab1b1e469cec4d0c4d747680742b73c37bbdd9b8fd81d8ec9\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"piggytest\",\"time\":1690622074,\"bolt11\":\"lnbc10n1pjvfkr6sp5rxqjqmufhp48etlsgxepwp33k2scctap3vvh63py3c5wtlvk93pspp56hp6sutk5z5f4vd3u35ua3xscnt5w6q8g2mncdammkdclkqa3myshp5gxwncgtpe3jmwprje9eyysh7ap0xe2ez8uy59s436xftc9vd0cdqxqzjccqpjrzjq0geslmtzh3zmklrmwe4v8l5fqv52y4wjs87nx9m9efxj74xrehh7rqafqqqwkqqqyqqqqlgqqqqqqgq2q9qxpqysgqflaf37unptvtzs738xalks6fz7xkh5jn0hem5xzmkgcznpcfa8fk6wtulumxmvpu7dzj440j20mvqjqjhhsr3y6td9asz7wlnh4080gq93wz75\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"d5c3a87176a0a89ab1b1e469cec4d0c4d747680742b73c37bbdd9b8fd81d8ec9\",\"expiry\":1690622674.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"hello there! Here's some sats!\"],\"wh_status\":404,\"wh_success\":false,\"wh_message\":\"Not Found\",\"wh_response\":\"response\"},\"wallet_id\":\"12345678901234567890123456789012\",\"webhook\":null,\"webhook_status\":null}]";
  // regular testcase with very long comment:
  // const String line = "[{\"checking_id\":\"eae170927e1e30811cb242a47436ec46aff9fbc73409079ce37f3e888bd45f7b\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"piggytest\",\"time\":1690630482,\"bolt11\":\"lnbc10n1pjvf72jsp5w99r5sg4kqnhjl2syltkwxf2gm86p0p3mh2rm5fwahxmwr047l3spp5atshpyn7rccgz89jg2j8gdhvg6hln778xsys088r0ulg3z75taashp5gxwncgtpe3jmwprje9eyysh7ap0xe2ez8uy59s436xftc9vd0cdqxqzjccqpjrzjqdjs3alg9wmchtfs0nav7nkls58u8usv5pc742q8fkw4gf9fpykqkzahvuqq2sgqqyqqqqqqqqqqqeqqjq9qxpqysgqttwctdvcq64s5tv0qemcykhw4atv7l3nta0029z75ve35xxk03mp6q2cs5yznnwp0euchrq7tw8heg98p7xylq9cl5jmd45r55zttfcp83pzun\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"eae170927e1e30811cb242a47436ec46aff9fbc73409079ce37f3e888bd45f7b\",\"expiry\":1690631082.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"This is a very long message, so let's see how it shows up. Woohoow! Looking great! BUT what if it was REALLY way too long to show, making the other text disappear? What will happen? Do we still show two messages? Let's find out!\"],\"wh_status\":404,\"wh_success\":false,\"wh_message\":\"Not Found\",\"wh_response\":\"response\"},\"wallet_id\":\"12345678901234567890123456789012\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"d5c3a87176a0a89ab1b1e469cec4d0c4d747680742b73c37bbdd9b8fd81d8ec9\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"piggytest\",\"time\":1690622074,\"bolt11\":\"lnbc10n1pjvfkr6sp5rxqjqmufhp48etlsgxepwp33k2scctap3vvh63py3c5wtlvk93pspp56hp6sutk5z5f4vd3u35ua3xscnt5w6q8g2mncdammkdclkqa3myshp5gxwncgtpe3jmwprje9eyysh7ap0xe2ez8uy59s436xftc9vd0cdqxqzjccqpjrzjq0geslmtzh3zmklrmwe4v8l5fqv52y4wjs87nx9m9efxj74xrehh7rqafqqqwkqqqyqqqqlgqqqqqqgq2q9qxpqysgqflaf37unptvtzs738xalks6fz7xkh5jn0hem5xzmkgcznpcfa8fk6wtulumxmvpu7dzj440j20mvqjqjhhsr3y6td9asz7wlnh4080gq93wz75\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"d5c3a87176a0a89ab1b1e469cec4d0c4d747680742b73c37bbdd9b8fd81d8ec9\",\"expiry\":1690622674.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"hello there! Here's some sats!\"],\"wh_status\":404,\"wh_success\":false,\"wh_message\":\"Not Found\",\"wh_response\":\"response\"},\"wallet_id\":\"12345678901234567890123456789012\",\"webhook\":null,\"webhook_status\":null}]";
  // regular testcase with extremely long comment:
  // const String line = "[{\"checking_id\":\"eae170927e1e30811cb242a47436ec46aff9fbc73409079ce37f3e888bd45f7b\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"piggytest\",\"time\":1690630482,\"bolt11\":\"lnbc10n1pjvf72jsp5w99r5sg4kqnhjl2syltkwxf2gm86p0p3mh2rm5fwahxmwr047l3spp5atshpyn7rccgz89jg2j8gdhvg6hln778xsys088r0ulg3z75taashp5gxwncgtpe3jmwprje9eyysh7ap0xe2ez8uy59s436xftc9vd0cdqxqzjccqpjrzjqdjs3alg9wmchtfs0nav7nkls58u8usv5pc742q8fkw4gf9fpykqkzahvuqq2sgqqyqqqqqqqqqqqeqqjq9qxpqysgqttwctdvcq64s5tv0qemcykhw4atv7l3nta0029z75ve35xxk03mp6q2cs5yznnwp0euchrq7tw8heg98p7xylq9cl5jmd45r55zttfcp83pzun\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"eae170927e1e30811cb242a47436ec46aff9fbc73409079ce37f3e888bd45f7b\",\"expiry\":1690631082.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"This is a very long message, so let's see how it shows up. Woohoow! Looking great! BUT what if it was REALLY way too long to show, making the other text disappear? What will happen? Do we still show two messages? Let's find out! WOW it is really capable of showing huge texts now, with this tiny font. So it needs to be even longer. And longer. Much longer! Now we are really pushing it beyond the limit of even the smallest font and the biggest display. So now something has got to give!\"],\"wh_status\":404,\"wh_success\":false,\"wh_message\":\"Not Found\",\"wh_response\":\"response\"},\"wallet_id\":\"12345678901234567890123456789012\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"d5c3a87176a0a89ab1b1e469cec4d0c4d747680742b73c37bbdd9b8fd81d8ec9\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"piggytest\",\"time\":1690622074,\"bolt11\":\"lnbc10n1pjvfkr6sp5rxqjqmufhp48etlsgxepwp33k2scctap3vvh63py3c5wtlvk93pspp56hp6sutk5z5f4vd3u35ua3xscnt5w6q8g2mncdammkdclkqa3myshp5gxwncgtpe3jmwprje9eyysh7ap0xe2ez8uy59s436xftc9vd0cdqxqzjccqpjrzjq0geslmtzh3zmklrmwe4v8l5fqv52y4wjs87nx9m9efxj74xrehh7rqafqqqwkqqqyqqqqlgqqqqqqgq2q9qxpqysgqflaf37unptvtzs738xalks6fz7xkh5jn0hem5xzmkgcznpcfa8fk6wtulumxmvpu7dzj440j20mvqjqjhhsr3y6td9asz7wlnh4080gq93wz75\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"d5c3a87176a0a89ab1b1e469cec4d0c4d747680742b73c37bbdd9b8fd81d8ec9\",\"expiry\":1690622674.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"hello there! Here's some sats!\"],\"wh_status\":404,\"wh_success\":false,\"wh_message\":\"Not Found\",\"wh_response\":\"response\"},\"wallet_id\":\"12345678901234567890123456789012\",\"webhook\":null,\"webhook_status\":null}]";
  // bitcoin3us' test case modified for 99999999 sats, one without comment, one with comment, both without webhooks:
  // const String line = "[{\"checking_id\":\"341e56153a2896b03ac653bdca88818bcc620be54d98faf1e56456e2c6caccdd\",\"pending\":false,\"amount\":99999999000,\"fee\":0,\"memo\":\"tip\",\"time\":1691438661,\"bolt11\":\"lnbc4140n1pjdznj9sp5ddzgugjfpqe9wpxu379rgn3sn6cpl2fnqk3h0wzjwga47rh5uwzqpp5xs09v9f69zttqwkx2w7u4zyp30xxyzl9fkv04u09v3tw93k2enwshp5z2jnl04hmynfk73xzsnrpm4dykcjrt6zf5lhk9rhfqg223q798xsxqzjccqpjrzjqf0wqzupw7dv2pt4f4fl4c2xkw6h0ctg8j95ax0vp3k0h38n7a7sgrpvq5qqzwgqqyqqqqlgqqqqp9sq2q9qxpqysgqex2w0mkcku6w80tc3l3uhvf076qq62sdk7sp374g6etvgnhrs4nkg73gecz4ed57677ccdhht9e8ej997vk27298p4gcjs40tfsz6cgpzn8uh2\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"341e56153a2896b03ac653bdca88818bcc620be54d98faf1e56456e2c6caccdd\",\"expiry\":1691439261.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"MB95To\",\"extra\":\"414000\"},\"wallet_id\":\"e5e8ce89a00a4dfeb461d6875888a4b2\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"af3bbf8463c6e5a444232f78f14138fb49bfdb096f448e70f5ff0f4878d452bb\",\"pending\":false,\"amount\":101000,\"fee\":0,\"memo\":\"tip\",\"time\":1689171770,\"bolt11\":\"lnbc1010n1pj2ade6sp54svuufvuwhzsp3c5fvxq0r0r7gzz4ggurvc3w8zmq9e6km7x2hvspp54uamlprrcmj6g3pr9au0zsfcldymlkcfdazguu84lu85s7x522ashp5z2jnl04hmynfk73xzsnrpm4dykcjrt6zf5lhk9rhfqg223q798xsxqzjccqpjrzjq2tt9ke59l8c0655mxqh2l7lf5l9gk74em6fr86ckhfcmlwh806ujz6cnsqqt5cqqqqqqqlgqqqcdgqq9q9qyysgqdrmqjx6x577dc8spylnuqefla4l4dtus2ksn3c9nmhx934wmsx9rkqpunzs9pp976mqj94v45x7xegp4jrsyp7efjwhas4e525lgqhqpgpkqze\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"af3bbf8463c6e5a444232f78f14138fb49bfdb096f448e70f5ff0f4878d452bb\",\"expiry\":1689172370.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"MB95To\",\"extra\":\"101000\",\"comment\":[\"Well done piggy!!\"]},\"wallet_id\":\"e5e8ce89a00a4dfeb461d6875888a4b2\",\"webhook\":null,\"webhook_status\":null}]";
  // one with comment, one without comment, both with webhook:
  // const String line = "[{\"checking_id\":\"30f89c969d07b1e9851911320e16dca655b2805aac5091978091228f7dc80dc0\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"piggytest\",\"time\":1692453842,\"bolt11\":\"lnbc10n1pjwpj7jsp5ng25uynapv0zggf3en87gx07nmd0fa3prp7d0ms7mdhgw7yvjp5spp5xrufe95aq7c7npgezyequ9ku5e2m9qz643gfr9uqjy3g7lwgphqqhp5gxwncgtpe3jmwprje9eyysh7ap0xe2ez8uy59s436xftc9vd0cdqxqzjccqpjrzjqf0wqzupw7dv2pt4f4fl4c2xkw6h0ctg8j95ax0vp3k0h38n7a7sgrpvq5qqzwgqqyqqqqlgqqqqp9sq2q9qxpqysgqzuzj2dctaz4knqqq32tjzdn943p5zphtexk04mhnyx2v2ysyazxq59vmdelsh79k8jtd7wez09y4zv33zlehlfj9jpperf9d8tnmm0gqrjrdhz\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"30f89c969d07b1e9851911320e16dca655b2805aac5091978091228f7dc80dc0\",\"expiry\":1692454442.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"Long comments are great for testing! BIG ones and small ones. Let's see how it gets displayed! Even if it is super long, it should still somehow show up.\"],\"wh_status\":200,\"wh_success\":true,\"wh_message\":\"OK\",\"wh_response\":\"POK\"},\"wallet_id\":\"5d59feaf97554bcd872c2bd686891d9e\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"8d8f9b6c959ffab9a1cb5f6505a307c5e1df0e4e880b260af1109817a03ea3b4\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"piggytest\",\"time\":1692456573,\"bolt11\":\"lnbc10n1pjwp4nasp5wdc2c3xh2ytvamgnnl82ujywg7v4gvk86yymzzep24kx3wp03mgqpp53k8ekmy4nlatngwttajstgc8chsa7rjw3q9jvzh3zzvp0gp75w6qhp5gxwncgtpe3jmwprje9eyysh7ap0xe2ez8uy59s436xftc9vd0cdqxqzjccqpjrzjqtxlgn2nt3cqqlmej5nqc89ua89d6hdrrtc8dvslgtc4agauknv2wzlytyqqy4gqqyqqqqqqqqqqxxcpyq9qxpqysgqqlmez48jpy88tepv6dxj9mqpv8he0vwg93md29y4g8neqs8cta7k38e3e43hdlrjsfd5lf4axp03fahkry7w9wtrp0cy43udqvrdepqqt4u9xg\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"8d8f9b6c959ffab9a1cb5f6505a307c5e1df0e4e880b260af1109817a03ea3b4\",\"expiry\":1692457173.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"wh_status\":200,\"wh_success\":true,\"wh_message\":\"OK\",\"wh_response\":\"POK\"},\"wallet_id\":\"5d59feaf97554bcd872c2bd686891d9e\",\"webhook\":null,\"webhook_status\":null}]";
  // many small ones:
  const String line = "[{\"checking_id\":\"dc8492bc5aa68c6de46f91c061438951a534d47aa0e30b9a05f31fd9737aad48\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"test Lightning Piggy\",\"time\":1711959072,\"bolt11\":\"lnbc10n1pnq5mpqsp5res7dugp64pktkh373ajekv3udsrted99u7cnnclrz743q4np8qspp5mjzf90z656xxmer0j8qxzsuf2xjnf4r65r3shxs97v0ajum644yqhp5v8tn7378jlmvzm5w2d0e4c6g7rutaynjl76ek5c4ndtsp7u3qsaqxqzjccqpjrzjqtty6qedwtvzdv9najlrt8c3yd5et27jfyd4s6xq40l59aaqxfafqrptd5qq84sqqyqqqqqqqqqqqqqq9q9qxpqysgqsntrzupf5uruvn6fna239j69zgmlufhnmyts7tfsxyajzq9dflpxnyv8pctc5ze9flrlg069pagz2u4734am5vjm7v8lsqlr5mc0pacqctu7px\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"dc8492bc5aa68c6de46f91c061438951a534d47aa0e30b9a05f31fd9737aad48\",\"expiry\":1711959672.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"again\"],\"lnaddress\":\"test@legend.lnbits.com\",\"wh_status\":200,\"wh_success\":true,\"wh_message\":\"OK\",\"wh_response\":\"POK\"},\"wallet_id\":\"5d59feaf97554bcd872c2bd686891d9e\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"f1636e5242467c96dbb6709e3900cab691b44cbc3d206357309e5ce3f04c8c89\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"test Lightning Piggy\",\"time\":1711918796,\"bolt11\":\"lnbc10n1pnqnnktsp5ugml8sv5ta2l8qn5rlsduz66s9qvey20wt4q79p6qw9uwcn9h0jspp5793ku5jzge7fdkakwz0rjqx2k6gmgn9u85sxx4esneww8uzv3jyshp5v8tn7378jlmvzm5w2d0e4c6g7rutaynjl76ek5c4ndtsp7u3qsaqxqzjccqpjrzjqdyukten642zgv4szeq95gklmgvxzlv840j8rrnpc3vsnw99gjfjjrqhwcqqdscqqsqqqqlgqqqzwyqqjq9qxpqysgq56z7am7ymw22cgmzv9gudlsn6l36wcpg8c8ecltvhlk2eerd32h93ch3c3r6hajdwns6q735kvd5pgj3rk3u22jvppsxk43w4eh07vcq6tsakv\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"f1636e5242467c96dbb6709e3900cab691b44cbc3d206357309e5ce3f04c8c89\",\"expiry\":1711919395.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"yeah\"],\"lnaddress\":\"test@legend.lnbits.com\",\"wh_status\":200,\"wh_success\":true,\"wh_message\":\"OK\",\"wh_response\":\"POK\"},\"wallet_id\":\"5d59feaf97554bcd872c2bd686891d9e\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"939336d326c3b1b25b46ce482aa02e403e73b9f5b9f6c3cb2331254f9167a491\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"test Lightning Piggy\",\"time\":1711899565,\"bolt11\":\"lnbc10n1pnqnqadsp58prjezh7zck55mqfar03phdcy46lczq9c40xjxwfr7ptme6jq4dspp5jwfnd5excwcmyk6xeeyz4gpwgql88w04h8mv8jerxyj5lyt85jgshp5v8tn7378jlmvzm5w2d0e4c6g7rutaynjl76ek5c4ndtsp7u3qsaqxqzjccqpjrzjqf40gxhsuwrphgtses8wlr695yq4zfw6c47z3h6nw5ku4m48jweg7rykm5qqv9cqqqqqqqqqqqqqq9gq2q9qxpqysgqmd896kuw9jjvf348j73h25fe0zdpx45hkl7pvv87k7gmhkgktfn8cgedx2dpvlmgz688nn3defervra9y7elwmle3dmqkcpftl8jaesp7wlvsd\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"939336d326c3b1b25b46ce482aa02e403e73b9f5b9f6c3cb2331254f9167a491\",\"expiry\":1711900165.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"GO\"],\"lnaddress\":\"test@legend.lnbits.com\",\"wh_status\":200,\"wh_success\":true,\"wh_message\":\"OK\",\"wh_response\":\"POK\"},\"wallet_id\":\"5d59feaf97554bcd872c2bd686891d9e\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"e940734d946b56bd05866ca768631073ecfcbe43a307eed2646905a9789a6c07\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"test Lightning Piggy\",\"time\":1711890271,\"bolt11\":\"lnbc10n1pnqjh6lsp54zmyurxlqu0q9cwtt007mlklmrgvjv2vfurzv5nstngffkusa3tqpp5a9q8xnv5ddtt6pvxdjnksccsw0k0e0jr5vr7a5nydyz6j7y6dsrshp5v8tn7378jlmvzm5w2d0e4c6g7rutaynjl76ek5c4ndtsp7u3qsaqxqzjccqpjrzjqf5zkjj9fl6jzrnecmrzj27ld3q9c6qf9kezkxpsn275pld37q6vsrxpwgqqd7gqqyqqqqlgqqqqq4gq2q9qxpqysgqnapmu4k22hxcl5lxlwptxcw4meupjff3ppuzhcjnrqkng4nawtszn48qxzp9jmnhsejhs2nqulx3cwa6gqunjcryv70uuek9qg0tsnqq87mpvv\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"e940734d946b56bd05866ca768631073ecfcbe43a307eed2646905a9789a6c07\",\"expiry\":1711890871.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"fast \"],\"lnaddress\":\"test@legend.lnbits.com\",\"wh_status\":200,\"wh_success\":true,\"wh_message\":\"OK\",\"wh_response\":\"POK\"},\"wallet_id\":\"5d59feaf97554bcd872c2bd686891d9e\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"8172c0f9f1b74f378d10676fc178897e3ea9ec441945957dcf129cd2bd5e77df\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"test Lightning Piggy\",\"time\":1711823496,\"bolt11\":\"lnbc10n1pnqsk5gsp570kdze8gvu2sramw2sywwrzwj6ktwwx2wz8q6xntn5n7tq8cpc2spp5s9evp703ka8n0rgsvahuz7yf0cl2nmzyr9ze2lw0z2wd9027wl0shp5v8tn7378jlmvzm5w2d0e4c6g7rutaynjl76ek5c4ndtsp7u3qsaqxqzjccqpjrzjq0f4w70lvyk9wjujf9xhgvqvu3n7p663qxq6dt37kaxc4k5frkp25zunqvqq85cqqqqqqqlgqqqq86qq0q9qxpqysgqq2jnj64kyye6lz52kn7x5xleukrld4gj5g6fu79upa8pke6wwz0krzfuv07nxlgv7nh2vq74enfee26ffz8xlzzvhtl988n65qqq3hcqpv04fg\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"8172c0f9f1b74f378d10676fc178897e3ea9ec441945957dcf129cd2bd5e77df\",\"expiry\":1711824096.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"Awesome!\"],\"lnaddress\":\"test@legend.lnbits.com\",\"wh_status\":200,\"wh_success\":true,\"wh_message\":\"OK\",\"wh_response\":\"POK\"},\"wallet_id\":\"5d59feaf97554bcd872c2bd686891d9e\",\"webhook\":null,\"webhook_status\":null},{\"checking_id\":\"74500fe1fadf0db1480e7a2c2b7eebb5c8076436fac07dc4b1a856a29cf7bae0\",\"pending\":false,\"amount\":1000,\"fee\":0,\"memo\":\"test Lightning Piggy\",\"time\":1711817178,\"bolt11\":\"lnbc10n1pnqsswesp520w7yq67hdm2cysag6se7ecwyv9plhk3t4eavk8p2q4k5v2wyjzqpp5w3gqlc06muxmzjqw0gkzklhtkhyqwepkltq8m3934pt2988hhtsqhp5v8tn7378jlmvzm5w2d0e4c6g7rutaynjl76ek5c4ndtsp7u3qsaqxqzjccqpjrzjqfcn85y6ghym65gf64csuc9xcre89j8679q6v4cv2p6t3us2pyeqwzl67gqq9nsqqqqqqqqqqqqqq9qq9q9qxpqysgqx3vnr33zlj2c3943kp49utlcdfkp80xy86rueua6gys7vlxpcu636wj4szeca8gts6m87eygjxcy6309yj7wgmjjx3vf0d0xm2z9ukqqjfsr3f\",\"preimage\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"payment_hash\":\"74500fe1fadf0db1480e7a2c2b7eebb5c8076436fac07dc4b1a856a29cf7bae0\",\"expiry\":1711817777.0,\"extra\":{\"tag\":\"lnurlp\",\"link\":\"5cvU6X\",\"extra\":\"1000\",\"comment\":[\"Amazing\"],\"lnaddress\":\"test@legend.lnbits.com\",\"wh_status\":200,\"wh_success\":true,\"wh_message\":\"OK\",\"wh_response\":\"POK\"},\"wallet_id\":\"5d59feaf97554bcd872c2bd686891d9e\",\"webhook\":null,\"webhook_status\":null}]";
  #else
  const String line = getEndpointData(lnbitsHost, url, true);
  #endif

  Serial.println("Got payments: " + line);

  DynamicJsonDocument doc(limit * 4096); // 4KB per lnurlpayment should be enough for everyone (tm)
  DeserializationError error = deserializeJson(doc, line);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  Serial.println("Displaying payment amounts and comments...");
  for (JsonObject areaElems : doc.as<JsonArray>()) {
    String paymentDetail = paymentJsonToString(areaElems);
    if (paymentDetail.length() > 0 && nroflnurlPayments<MAX_PAYMENTS) {
      lnurlPayments[nroflnurlPayments] = paymentDetail;
      nroflnurlPayments++;
    }
  }

  Serial.println("After parsing LNURL payments, the list contains:" + stringArrayToString(lnurlPayments, nroflnurlPayments));
}


String getLNURLp() {
  return getLNURLp(false); // mustFetchWalletID = false
}

/**
 * @brief Get the first available LNURLp from the wallet
 *
 * if mustFetch is true then the return value (String LNURLp) isn't used
 * but rather, the walletIDfromLNURLp is what will be used later.
 *
 * @return lnurlp for accepting payments
 *
 */
String getLNURLp(bool mustFetchWalletID) {
  #ifdef DEBUG
  Serial.println("Mocking getLNURLp:"); return "LNURL1DP68GURN8GHJ7MR9VAJKUEPWD3HXY6T5WVHXXMMD9AKXUATJD3CZ7DTRWE2NVKQ72L5D3";
  #endif

  // Only fetch the first one using the API if no fixed lnurlPayments was configured (unless mustFetchWalletID)
  if (isConfigured(staticLNURLp) && !mustFetchWalletID) {
    return staticLNURLp;
  }

  Serial.println("Getting LNURLp link list...");

  // Get the first lnurlp
  String lnurlpData = getEndpointData(lnbitsHost, "/lnurlp/api/v1/links?all_wallets=false", true);
  DynamicJsonDocument doc(8192); // the size of the list of links is unknown so don't skimp here

  DeserializationError error = deserializeJson(doc, lnurlpData);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
  }
  String lnurlpId = doc[0]["id"];
  String lnurlp = doc[0]["lnurl"];
  String localWalletIDfromLNURLp = doc[0]["wallet"];
  setFoundWalletID(localWalletIDfromLNURLp);

  Serial.println("Fetched LNURLp: " + lnurlp + " and found LNURLp wallet ID:" + localWalletIDfromLNURLp);
  return lnurlp;
}

int getBalanceBiasAsInt() {
  int balanceBiasInt = 0;
  if (isConfigured(balanceBias)) {
    if (str2int(&balanceBiasInt, (char*)balanceBias, 10) != STR2INT_SUCCESS) {
      Serial.println("WARNING: failed to convert balanceBias ('" + String(balanceBias) + "') to integer, ignoring...");
    } else {
      Serial.println("Adding balanceBias value of " + String(balanceBiasInt) + " to balance.");
    }
  }
  return balanceBiasInt;
}

void setFoundWalletID(String walletID) {
  if (foundWalletID.length() == 0) {
    foundWalletID = walletID;
    Serial.println("Setting foundWalletID = " + String(walletID));
  } else {
    Serial.println("Not setting foundWalletID = " + String(walletID));
  }
}

// Returns the wallet ID from configuration,
// or the wallet ID that was found
// either in the LNURLp list or in an incoming payment.
String getWalletID() {
  if (isConfigured(walletID)) {
    return walletID;
  } else {
    return foundWalletID;
  }
}

void addLNURLpayment(String toadd) {
  // first move them all down one spot
  for (int i=min(nroflnurlPayments-1,MAX_PAYMENTS-1);i>0;i--) {
    lnurlPayments[i] = lnurlPayments[i-1];
  }
  lnurlPayments[0] = toadd;
  if (nroflnurlPayments<MAX_PAYMENTS) nroflnurlPayments++;
  Serial.println("After parsing LNURL payments, the list contains:" + stringArrayToString(lnurlPayments, nroflnurlPayments));
}

int getNroflnurlPayments() {
  return nroflnurlPayments;
}

String getLnurlPayment(int item) {
  return lnurlPayments[item];
}
