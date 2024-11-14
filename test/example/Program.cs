using System;

namespace example {
 
    static class Program
    {
        private static string test(string value) {
            Console.WriteLine(value);

            return "Hello";
        }
        
        static void Main()
        {
            Console.WriteLine(test("Test"));
        }
    }
    
}