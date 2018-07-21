//  {"NAME",
//   {{INPUT TYPES...},
//    [](Stack& s, map<string, DefinedFunction>&) {
//      PRIMITIVE BODY...
//    }}},
{"add",
 {{StackElement::DataType::Number, StackElement::DataType::Number},
  [](Stack& s, map<string, DefinedFunction>&) {
    NumberElement* lower = dynamic_cast<NumberElement*>(s.pop());
    NumberElement* higher = dynamic_cast<NumberElement*>(s.pop());
    NumberElement* result =
        new NumberElement(lower->getData() + higher->getData());
    delete lower;
    delete higher;
    s.push(result);
  }}},
    {"subtract",
     {{StackElement::DataType::Number, StackElement::DataType::Number},
      [](Stack& s, map<string, DefinedFunction>&) {
        NumberElement* lower = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* higher = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* result =
            new NumberElement(higher->getData() - lower->getData());
        delete lower;
        delete higher;
        s.push(result);
      }}},
    {"multiply",
     {{StackElement::DataType::Number, StackElement::DataType::Number},
      [](Stack& s, map<string, DefinedFunction>&) {
        NumberElement* lower = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* higher = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* result =
            new NumberElement(higher->getData() * lower->getData());
        delete lower;
        delete higher;
        s.push(result);
      }}},
    {"divide",
     {{StackElement::DataType::Number, StackElement::DataType::Number},
      [](Stack& s, map<string, DefinedFunction>&) {
        if (dynamic_cast<NumberElement*>(s.top())->getData() == 0) {
          throw SyntaxError("Attempted division by zero.");
        }
        NumberElement* lower = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* higher = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* result =
            new NumberElement(higher->getData() / lower->getData());
        delete lower;
        delete higher;
        s.push(result);
      }}},
    {"negate",
     {{StackElement::DataType::Number},
      [](Stack& s, map<string, DefinedFunction>&) {
        NumberElement* input = dynamic_cast<NumberElement*>(s.pop());
        NumberElement* result = new NumberElement(-input->getData());
        delete input;
        s.push(result);
      }}},