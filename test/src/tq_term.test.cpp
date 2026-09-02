#include "qtest.hpp"

#include "tq_term.h"

SCENARIO_START

DESCRIBE("termiq::detail::base64_encode/decode", {
	IT("should encode and decode the message", {
		std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum dapibus ligula pretium vulputate volutpat. Proin congue risus vel magna finibus malesuada. Ut efficitur gravida dignissim. Cras tincidunt, lorem sed gravida aliquam, risus erat suscipit augue, id egestas odio eros sit amet quam. Vestibulum a gravida enim, vel eleifend libero. Sed aliquam consectetur nibh. Nulla lorem nisi, tempor sit amet magna sed, euismod euismod ligula.";
		for (size_t i=1;i<text.size();++i) {
			std::string encoded = termiq::detail::base64_encode(std::string_view(text.begin(), text.begin()+i));
			std::string decoded = termiq::detail::base64_decode(encoded);

			EXPECT(decoded).toBeIterableEqual(std::string_view(text.begin(), text.begin()+i));
		}
	});
});

SCENARIO_END
