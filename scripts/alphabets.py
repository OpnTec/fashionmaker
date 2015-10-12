#!/usr/bin/env python
# -*- coding: utf-8 -*-
import codecs
import sys
#Alphabets was taken from website http://www.alphabet-letters.com/

#Use this script if you want get all unique symbols from all alphabets.
#This unique symbols need for math parser.
#Example:
# ցЀĆЈVӧĎАғΕĖӅИқΝĞơРңњΥĦШҫ̆جگĮаҳѕεشԶиһνԾрυلՆӝшËՎҔPÓՖXӛӟŞӣզhëծpóӞնxßվāŁЃֆĉЋCŬđҐГΒęҘЛΚŘġҠУGاհЫدԱҰгβطԹõлκKՁÀуςهՉÈыvیՑÐSOřӘћաőcӐթèkàѓżűðsķչøӥӔĀփїІĈЎґĐΗЖҙĘȚ
# ΟОҡĠآΧЦتЮұİزηжԸغοоÁՀقχцÉՈيюÑՐђӋіәťӆўáŠĺѐfөըnñŰӤӨӹոľЁրăЉŭċБӸēłΔҖЙŤěΜӜDСձģΤӰЩīņحҮбưԳصδHйԻŇμӲӴсՃمτƠщՋєLQŹՓŕÖYśÞaգĽæiŽիӓîqճöyջþĂօЄӦĊЌΑĒДҗјΙȘĚМΡéĵĢФūӚΩبĪЬүќ
# αذԲдҷιظԺмρՂфÇωوՊьÏՒTŚĻJբdçժlïӪղtպӫAւąЇčŃЏĕӯЗΖEțŮĝПΞأĥĹЧΦثÆӳЯIسŲԵзζԽпξكՅÄчφNMՍӌяӢՕÔWÎŝÜџёźեägխoӒյôwĶBžսüЂĄև̈ЊČƏљΓВҕĔӮΛКĜΣТҥĤکЪƯخγвŅԴŪضλкԼĴσтÅՄنъÍՌRӕ
# ՔZÝŜbåդﻩjíլļrӵմzýռپêЅքćچЍďӱҒЕůėژșΘØҚНğńءΠFҢХħΨҪЭųįҶرҲеԷňعθҺнԿفπÂхՇψÊэšՏÒUəÚѝŻşҤӑâeէŐımկòuշÕúտŔ

RUSSIAN_ALPHABET     = set(u'абвгдеёжзийклмнопрстуфхцчшщьыъэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯ')                                           #RUSSIAN ALPHABET
ENGLISH_ALPHABET     = set(u'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')                                                         #ENGLISH ALPHABET
AZERBAIJANI_ALPHABET = set(u'abcçdeəfgğhxıijkqlmnoöprsştuüvyzABCÇDEƏFGĞHXIİJKQLMNOÖPRSŞTUÜVYZ')                                             #AZERBAIJANI ALPHABET
ALBANIAN_ALPHABET    = set(u'abcçddheëfggjhijklllmnnjopqrrrsshtthuvxxhyzzhABCÇDDhEËFGGjHIJKLLlMNNjOPQRRrSShTThUVXXhYZZh')                   #ALBANIAN ALPHABET
ARABIC_ALPHABET      = set(u'أبجدوﻩزحطيكلمنسعفصقرشتثخذضظغ')                                                                                #ARABIC ALPHABET
ARMENIAN_ALPHABET    = set(u'աբգդեզէըթժիլխծկհձղճմյնշոչպջռսվտրցւփքեւօֆԱԲԳԴԵԶԷԸԹԺԻԼԽԾԿՀՁՂՃՄՅՆՇՈՉՊՋՌՍՎՏՐՑՒՓՔԵվևՕՖ') #ARMENIAN ALPHABET
AFRIKAANS_ALPHABET   = set(u'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')                                                         #AFRIKAANS ALPHABET
BASQUE_ALPHABET      = set(u'аbdefghijklmnñoprstuxzАBDEFGHIJKLMNÑOPRSTUXZ')                                                                 #BASQUE ALPHABET
BASHKIR_ALPHABET     = set(u'абвгғдҙеёжзийкҡлмнңоөпрсҫтуүфхһцчшщъыьэәюяАБВГҒДҘЕЁЖЗИЙКҠЛМНҢОӨПРСҪТУҮФХҺЦЧШЩЪЫЬЭӘЮЯ')                         #BASHKIR ALPHABET
BELORUSSIAN_ALPHABET = set(u'абвгддждзеёжзійклмнопрстуўфхцчшыьэюяАБВГДДжДзЕЁЖЗІЙКЛМНОПРСТУЎФХЦЧШЫЬЭЮЯ')                                     #BELORUSSIAN ALPHABET
BULGARIAN_ALPHABET   = set(u'абвгдежзийклмнопрстуфхцчшщъьюяАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЬЮЯ')                                                 #BULGARIAN ALPHABET
BURYATIAN_ALPHABET   = set(u'абвгдеёжзийклмноөпрстуүфхһцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОӨПРСТУҮФХҺЦЧШЩЪЫЬЭЮЯ')                                     #BURYATIAN ALPHABET
WELSH_ALPHABET       = set(u'abcchdddefffgnghilllmnopphrrhstthuwyABCCHDDDEFFFGNGHILLLMNOPPHRRHSTTHUWY')                                     #WELSH ALPHABET
HUNGARIAN_ALPHABET   = set(u'aábccsddzdzseéfggyhiíjkllymnnyoóöőprsszttyuúüűvzzsqwxyAÁBCCsDDzDzsEÉFGGyHIÍJKLLyMNNyOÓÖŐPRSSzTTyUÚÜŰVZZsQWXY') #HUNGARIAN ALPHABET
VIETNAMESE_ALPHABET  = set(u'aăâbcdđeêghiklmnoôơpqrstuưvxyAĂÂBCDĐEÊGHIKLMNOÔƠPQRSTUƯVXY')                                                   #VIETNAMESE ALPHABET
HAITIAN_ALPHABET     = set(u'aanbchdeèenfghijklmnngoòonouounprstuivwyzAAnBChDEèEnFGHIJKLMNNgOòOnOuOunPRSTUiVWYZ')                           #HAITIAN ALPHABET
GALICIAN_ALPHABET    = set(u'abcdefghilmnñopqrstuvxzABCDEFGHILMNÑOPQRSTUVXZ')                                                               #GALICIAN ALPHABET
DUTCH_ALPHABET       = set(u'abcdefghijklmnopqrstuvwxijyzABCDEFGHIJKLMNOPQRSTUVWXIJYZ')                                                     #DUTCH ALPHABET
GREEK_ALPHABET       = set(u'αβγδεζηθικλμνξοπρσςτυφχψωΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩ')															#GREEK ALPHABET
DANISH_ALPHABET      = set(u'аbcdefghijklmnopqrstuvwxyzæøåАBCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ')                                                   #DANISH ALPHABET
INDONESIAN_ALPHABET  = set(u'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')                                                         #INDONESIAN ALPHABET
IRISH_ALPHABET       = set(u'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')                                                         #IRISH ALPHABET
ICELANDIC_ALPHABET   = set(u'aábdðeéfghiíjklmnoóprstuúvxyýþæöAÁBDÐEÉFGHIÍJKLMNOÓPRSTUÚVXYÝÞÆÖ')                                             #ICELANDIC ALPHABET
SPANISH_ALPHABET     = set(u'abcdefghijklmnñopqrstuvwxyzABCDEFGHIJKLMNÑOPQRSTUVWXYZ')                                                       #SPANISH ALPHABET
ITALIAN_ALPHABET     = set(u'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')                                                         #ITALIAN ALPHABET
KAZAKH_ALPHABET      = set(u'аәбвгғдеёжзийкқлмнңоөпрстуұүфхһцчшщъыіьэюяАӘБВГҒДЕЁЖЗИЙКҚЛМНҢОӨПРСТУҰҮФХҺЦЧШЩЪЫІЬЭЮЯ')                         #KAZAKH ALPHABET
KALMYK_ALPHABET      = set(u'аəбвгһдеёжҗзийклмнңоөпрстуүфхцчшщыьэюяАƏБВГҺДЕЁЖҖЗИЙКЛМНҢОӨПРСТУҮФХЦЧШЩЫЬЭЮЯ')                                 #KALMYK ALPHABET
KARAKALPAK_ALPHABET  = set(u'аәбвгғдеёжзийкқлмнңоөпрстуүўфхҳцчшщъыьэюяАӘБВГҒДЕЁЖЗИЙКҚЛМНҢОӨПРСТУҮЎФХҲЦЧШЩЪЫЬЭЮЯ')                           #KARAKALPAK ALPHABET
KARELIAN_ALPHABET    = set(u'abčdefghijklmnoprsšzžtuvyäöABČDEFGHIJKLMNOPRSŠZŽTUVYÄÖ')                                                       #KARELIAN ALPHABET
CATALAN_ALPHABET     = set(u'aàbcçdeéèfghiíïjklmnoóòpqrstuúüvwxyzAÀBCÇDEÉÈFGHIÍÏJKLMNOÓÒPQRSTUÚÜVWXYZ')                                     #CATALAN ALPHABET
KYRGYZ_ALPHABET      = set(u'абвгдеёжзийклмнңоөпрстуүфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНҢОӨПРСТУҮФХЦЧШЩЪЫЬЭЮЯ')									    #KYRGYZ ALPHABET
LATIN_ALPHABET       = set(u'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')                                                         #LATIN ALPHABET
LATVIAN_ALPHABET     = set(u'aābcčdeēfgģhiījkķlļmnņoprsštuūvzžAĀBCČDEĒFGĢHIĪJKĶLĻMNŅOPRSŠTUŪVZŽ')                                           #LATVIAN ALPHABET
LITHUANIAN_ALPHABET  = set(u'aąbcčdeęėfghiįyjklmnoprsštuųūvzžAĄBCČDEĘĖFGHIĮYJKLMNOPRSŠTUŲŪVZŽ')                                             #LITHUANIAN ALPHABET
MACEDONIAN_ALPHABET  = set(u'абвгдѓеѐжзѕиѝјклљмнњопрстќуфхцчџшАБВГДЃЕЀЖЗЅИЍЈКЛЉМНЊОПРСТЌУФХЦЧЏШ')                                           #MACEDONIAN ALPHABET
MALTESE_ALPHABET     = set(u'abċdefġggħhħiiejklmnopqrstuvwxzżABĊDEFĠGGĦHĦIIEJKLMNOPQRSTUVWXZŻ')                                             #MALTESE ALPHABET
MARI_ALPHABET        = set(u'аӓбвгдеёжзийклмноӧпрстуӱфхцчшщъыӹьэюяАӒБВГДЕЁЖЗИЙКЛМНОӦПРСТУӰФХЦЧШЩЪЫӸЬЭЮЯ')                                   #MARI ALPHABET
MOLDAVIAN_ALPHABET   = set(u'aăâbcdefghiîjklmnopqrsștțuvwxyzAĂÂBCDEFGHIÎJKLMNOPQRSȘTȚUVWXYZ')                                               #MOLDAVIAN ALPHABET
MONGOLIAN_ALPHABET   = set(u'абвгдеёжзийклмноөпрстуүфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОӨПРСТУҮФХЦЧШЩЪЫЬЭЮЯ')                                       #MONGOLIAN ALPHABET
GERMAN_ALPHABET      = set(u'aäbcdefghijklmnoöpqrstuüvwxyzAÄBCDEFGHIJKLMNOÖPQRSßTUÜVWXYZ')                                                  #GERMAN ALPHABET
NORWEGIAN_ALPHABET   = set(u'abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ')                                                   #NORWEGIAN ALPHABET
OSETIAN_ALPHABET     = set(u'аӕбвггъддзджеёжзийккълмноппърсттъуфххъццъччъшщъыьэюяАӔБВГГъДДзДжЕЁЖЗИЙККъЛМНОППъРСТТъУФХХъЦЦъЧЧъШЩъЫьЭЮЯ')     #OSETIAN ALPHABET
POLISH_ALPHABET      = set(u'aąbcćdeęfghijklłmnńoóprsśtuwyzźżAĄBCĆDEĘFGHIJKLŁMNŃOÓPRSŚTUWYZŹŻ')                                             #POLISH ALPHABET
PORTUGUESE_ALPHABET  = set(u'aáàãâbcçdeéêẽfghiíjklmnoóôãpqrstuvwxyzÁÀÃÂABCÇDEÉÊFGHIÍJKLMNÑOÕÓPQRSTUVWXYZ')                                                         #PORTUGUESE ALPHABET
ROMANIAN_ALPHABET    = set(u'aăâbcdefghiîjklmnopqrsștțuvwxyzAĂÂBCDEFGHIÎJKLMNOPQRSȘTȚUVWXYZ')                                               #ROMANIAN ALPHABET
SERBIAN_ALPHABET     = set(u'абвгдђежзијклљмнњопрстћуфхцчџшАБВГДЂЕЖЗИЈКЛЉМНЊОПРСТЋУФХЦЧЏШ')                                                 #SERBIAN ALPHABET
SLOVAK_ALPHABET      = set(u'aáäbcčdďdzdžeéfghchiíjklĺľmnňoóôpqrŕsštťuúvwxyýzžAÁÄBCČDĎDzDžEÉFGHChIÍJKLĹĽMNŇOÓÔPQRŔSŠTŤUÚVWXYÝZŽ')           #SLOVAK ALPHABET
SLOVENIAN_ALPHABET   = set(u'abcčddžefghijklljmnnjoprsštuvzžABCČDDžEFGHIJKLLjMNNjOPRSŠTUVZŽ')                                               #SLOVENIAN ALPHABET
TAJIK_ALPHABET       = set(u'абвгғдеёжзийӣкқлмнопрстуӯфхҳчҷшъэюяАБВГҒДЕЁЖЗИЙӢКҚЛМНОПРСТУӮФХҲЧҶШЪЭЮЯ')                                       #TAJIK ALPHABET
TATAR_ALPHABET       = set(u'аәбвгдеёжҗзийклмнңоөпрстуүфхһцчшщъыьэюяАӘБВГДЕЁЖҖЗИЙКЛМНҢОӨПРСТУҮФХҺЦЧШЩЪЫЬЭЮ')                                #TATAR ALPHABET
TUVIN_ALPHABET       = set(u'абвгдёжӝзийклмнңоӧпрстуӱхчшыАБВГДЁЖӜЗИЙКЛМНҢОӦПРСТУӰХЧШЫ')                                                     #TUVIN ALPHABET
TURKISH_ALPHABET     = set(u'abcçdefgğhiıjklmnoöpqrsştuüvwxyzABCÇDEFGĞHIIJKLMNOÖPQRSŞTUÜVWXYZ')                                             #TURKISH ALPHABET
UDMURT_ALPHABET      = set(u'абвгдеёжӝзӟиӥйклмноӧпрстуфхцчӵшщъыьэюяАБВГДЕЁЖӜЗӞИӤЙКЛМНОӦПРСТУФХЦЧӴШЩЪЫЬЭЮЯ')                                 #UDMURT ALPHABET
UZBEK_ALPHABET       = set(u'абвгғдеёжзийкқлмнопрстуўфхҳцчшъьэюяАБВГҒДЕЁЖЗИЙКҚЛМНОПРСТУЎФХҲЦЧШЪЬЭЮЯ')                                       #UZBEK ALPHABET
UKRAINIAN_ALPHABET   = set(u'абвгґдежзіїийклмнопрстуфхцчшщьєюяАБВГҐДЕЖЗІЇИЙКЛМНОПРСТУФХЦЧШЩЬЄЮЯ')                                           #UKRAINIAN ALPHABET
FARSI_ALPHABET       = set(u'ءاآبپتثجچحخدذرزژسشصضطظعغفقكکگلمنوهيی')                                                                        #FARSI ALPHABET
PHILIPPINES_ALPHABET = set(u'abcdefghijklmnñngoprstuvwxyzABCDEFGHIJKLMNÑNgOPRSTUVWXYZ')                                                     #PHILIPPINES ALPHABET
FINNISH_ALPHABET     = set(u'abcdefghijklmnopqrsštuvwxyzžåäöABCDEFGHIJKLMNOPQRSŠTUVWXYZŽÅÄÖ')                                               #FINNISH ALPHABET
FRENCH_ALPHABET      = set(u'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')                                                         #FRENCH ALPHABET
HAKASS_ALPHABET      = set(u'абвгғдеёжзиiйклмнңoӧпрстуӱфхцчӌшщъыьэюяАБВГҒДЕЁЖЗИIЙКЛМНҢOӦПРСТУӰФХЦЧӋШЩЪЫЬЭЮЯ')                               #HAKASS ALPHABET
HANTY_ALPHABET       = set(u'аӓӑбвгдеёәӛжзийкқлӆмнңн̆оӧөӫпрстуӱўфхҳцчҷшщъыьэєє̈юю̆яя̆АӒӐБВГДЕЁӘӚЖЗИЙКҚЛӅМНҢН̆ОӦӨӪПРСТУӰЎФХҲЦЧҶШЩЪЫЬЭЄЄ̈ЮЮ̆ЯЯ̆')     #HANTY ALPHABET
CROATIAN_ALPHABET    = set(u'abcčćddžðefghijklljmnnjoprsštuvzžABCČĆDDžÐEFGHIJKLLjMNNjOPRSŠTUVZŽ')                                           #CROATIAN ALPHABET
BOSNIAN_ALPHABET     = set(u'abcčćddžðefghijklljmnnjoprsštuvzžABCČĆDDžÐEFGHIJKLLjMNNjOPRSŠTUVZŽ')                                           #BOSNIAN ALPHABET
CZECH_ALPHABET       = set(u'aábcčdďeéěfghchiíjklmnňoópqrřsštťuúůvwxyýzžAÁBCČDĎEÉĚFGHChIÍJKLMNŇOÓPQRŘSŠTŤUÚŮVWXYÝZŽ')                       #CZECH ALPHABET
CHUVASH_ALPHABET     = set(u'аăбвгдеёĕжзийклмнопрсçтуӳфхцчшщьыъэюяАĂБВГДЕЁĔЖЗИЙКЛМНОПРСÇТУӲФХЦЧШЩЬЫЪЭЮЯ')                                   #CHUVASH ALPHABET
SWEDISH_ALPHABET     = set(u'aåäbcdefghijklmnoöpqrstuvwxyzAÅÄBCDEFGHIJKLMNOÖPQRSTUVWXYZ')                                                   #SWEDISH ALPHABET
ESPERANTO_ALPHABET   = set(u'abcĉdefgĝhĥijĵklmnoprsŝtuŭvzABCĈDEFGĜHĤIJĴKLMNOPRSŜTUŬVZ')                                                     #ESPERANTO ALPHABET
ESTONIAN_ALPHABET    = set(u'abcdefghijklmnopqrsšzžtuvwõäöüxyABCDEFGHIJKLMNOPQRSŠZŽTUVWÕÄÖÜXY')                                             #ESTONIAN ALPHABET
YAKUTIAN_ALPHABET    = set(u'абвгҕддьеёжзийклмнҥньоөпрсһтуүфхцчшщъыьэАБВГҔДДьЕЁЖЗИЙКЛМНҤНьОӨПРСҺТУҮФХЦЧШЩЪЫЬЭ')                             #YAKUTIAN ALPHABET
MONTENEGRIN_ALPHABET = set(u'abcčćddžðefghijklljmnnjoprsštuvzžABCČĆDDžÐEFGHIJKLLjMNNjOPRSŠTUVZŽ')                                           #MONTENEGRIN ALPHABET

UTF8Writer = codecs.getwriter('utf8')
sys.stdout = UTF8Writer(sys.stdout)

SYMBOLS = set(RUSSIAN_ALPHABET)
SYMBOLS.update(RUSSIAN_ALPHABET, ENGLISH_ALPHABET, AZERBAIJANI_ALPHABET, ALBANIAN_ALPHABET, ARABIC_ALPHABET, ARMENIAN_ALPHABET, AFRIKAANS_ALPHABET, BASQUE_ALPHABET, BASHKIR_ALPHABET,
BELORUSSIAN_ALPHABET, BULGARIAN_ALPHABET, BURYATIAN_ALPHABET, WELSH_ALPHABET, HUNGARIAN_ALPHABET, VIETNAMESE_ALPHABET, HAITIAN_ALPHABET, GALICIAN_ALPHABET, DUTCH_ALPHABET, GREEK_ALPHABET,
DANISH_ALPHABET, INDONESIAN_ALPHABET, IRISH_ALPHABET, ICELANDIC_ALPHABET, SPANISH_ALPHABET, ITALIAN_ALPHABET, KAZAKH_ALPHABET, KALMYK_ALPHABET, KARAKALPAK_ALPHABET, KARELIAN_ALPHABET,
CATALAN_ALPHABET, KYRGYZ_ALPHABET, LATIN_ALPHABET, LATVIAN_ALPHABET, LITHUANIAN_ALPHABET, MACEDONIAN_ALPHABET, MALTESE_ALPHABET, MARI_ALPHABET, MOLDAVIAN_ALPHABET, MONGOLIAN_ALPHABET,
GERMAN_ALPHABET, NORWEGIAN_ALPHABET, OSETIAN_ALPHABET, POLISH_ALPHABET, PORTUGUESE_ALPHABET, ROMANIAN_ALPHABET, SERBIAN_ALPHABET, SLOVAK_ALPHABET, SLOVENIAN_ALPHABET, TAJIK_ALPHABET, TATAR_ALPHABET,
TUVIN_ALPHABET, TURKISH_ALPHABET, UDMURT_ALPHABET, UZBEK_ALPHABET, UKRAINIAN_ALPHABET, FARSI_ALPHABET, PHILIPPINES_ALPHABET, FINNISH_ALPHABET, FRENCH_ALPHABET, HAKASS_ALPHABET,HANTY_ALPHABET,
BOSNIAN_ALPHABET, CROATIAN_ALPHABET, CZECH_ALPHABET, CHUVASH_ALPHABET, SWEDISH_ALPHABET, ESPERANTO_ALPHABET, ESTONIAN_ALPHABET, YAKUTIAN_ALPHABET, MONTENEGRIN_ALPHABET)

L = list(SYMBOLS)
print ''.join(L)

