/* DO NOT CHANGE:  This file is used in evaluation */

public class Stack extends List {
        
        public Stack(String name) {
                super(name);
        }

        public Base pop () {
                return remove (END);
        }

        public Base push (Base element) {
                return insert (element, END);
        }

        public Base top () {
                return view (END);
        }
}
