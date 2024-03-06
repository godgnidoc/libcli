import { Option } from "./option";
export type CliArg = string | CliOption | CliElePath;
export type CliOption = {
    is: 'option';
    keyword: string;
    define: Option;
    value: boolean | string | string[];
};
export type CliElePath = {
    is: 'path';
    path: string[];
};
export type ArgPass = (arg: string) => Error | boolean;
export declare function parseOptionArg(option: CliOption, args: CliArg[], start: number): Promise<boolean>;
